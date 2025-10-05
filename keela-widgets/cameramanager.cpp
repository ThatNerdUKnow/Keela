//
// Created by brand on 6/3/2025.
//

#include "keela-widgets/cameramanager.h"

#include <keela-pipeline/utils.h>
#include <spdlog/spdlog.h>

#include <ctime>
#include <filesystem>
#include <sstream>
#include <stdexcept>

#include "keela-pipeline/recordbin.h"
#include "keela-widgets/plugin_utils.h"

Keela::CameraManager::CameraManager(guint id, std::string pix_fmt, bool split_streams) : Bin("camera_" + std::to_string(id)), camera(Keela::get_video_test_source_name()) {
    try {
        spdlog::info("Creating camera manager {}", id);
        this->id = id;

        // Add all elements to the bin (except camera_stream_even which has deleted copy constructor)
        add_elements(camera, caps_filter, transform, tee_main);

        // @TODO: figure out how to do this more cleanly
        // Add camera_stream_even manually due to deleted copy constructor from multiple inheritance
        // Cast to Bin base class first to resolve ambiguous conversion
        Keela::Bin *camera_stream_bin = camera_stream_even.get();
        GstElement *camera_stream_elem_even = static_cast<GstElement *>(*camera_stream_bin);
        gst_object_ref(camera_stream_elem_even);
        GstBin *this_bin = static_cast<GstBin *>(*this);
        gboolean ret = gst_bin_add(this_bin, camera_stream_elem_even);
        if (!ret) {
            throw std::runtime_error("Failed to add camera_stream_even to bin");
        }
        // Link main tee to camera_stream_even manually using gst_element_link
        gboolean link_result = gst_element_link(tee_main, camera_stream_elem_even);
        if (!link_result) {
            throw std::runtime_error("Failed to link tee_main to camera_stream_even");
        }

        // Link main pipeline: camera -> capsfilter -> transform -> main_tee
        element_link_many(camera, caps_filter, transform, tee_main);

        if (split_streams) {
            add_odd_camera_stream();
        }

        // Set up frame splitting if enabled
        this->split_streams = split_streams;
        if (split_streams) {
            install_frame_splitting_probes();
        }

        set_pix_fmt(pix_fmt);
        spdlog::info("Created camera manager {}", id);
    } catch (const std::exception &e) {
        std::stringstream ss;

        ss << "camera" << id << ".dot";
        spdlog::error("could not create camera manager: {}\n attempting to dump bin to {}", e.what(), ss.str());
        dump_bin_graph();
        throw;
    }
}

Keela::CameraManager::~CameraManager() {
    spdlog::debug(__func__);
}

void Keela::CameraManager::set_pix_fmt(const std::string &format) {
    spdlog::info("{}: Setting pixel format to {}", __func__, format);
    // create copy of our caps
    base_caps = Caps(static_cast<GstCaps *>(base_caps));
    // apply pixel format
    gst_caps_set_simple(base_caps, "format", G_TYPE_STRING, format.c_str(), nullptr);
    g_object_set(caps_filter, "caps", static_cast<GstCaps *>(base_caps), nullptr);
}

void Keela::CameraManager::set_framerate(double framerate) {
    spdlog::info("Setting framerate to {}", framerate);
    int numerator = static_cast<int>(framerate * 10);
    // TODO: caps need to be writable
    base_caps = Caps(static_cast<GstCaps *>(base_caps));
    base_caps.set_framerate(numerator, 10);

    // through experimentation, I believe that changes to the original caps reference do not affect the capsfilter
    g_object_set(caps_filter, "caps", static_cast<GstCaps *>(base_caps), nullptr);
}

void Keela::CameraManager::set_resolution(const int width, const int height) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("width and height must be greater than zero");
    }
    // create a copy of our current caps
    base_caps = Caps(static_cast<GstCaps *>(base_caps));
    base_caps.set_resolution(width, height);
    g_object_set(caps_filter, "caps", static_cast<GstCaps *>(base_caps), nullptr);
    transform.scale(width, height);
}

void Keela::CameraManager::set_experiment_directory(const std::string &path) {
    experiment_directory = path;
}

void Keela::CameraManager::start_recording() {
    std::string suffix = split_streams ? "even" : "";

    camera_stream_even->start_recording(get_filename(experiment_directory, this->id, suffix));

    if (split_streams) {
        camera_stream_odd->start_recording(get_filename(experiment_directory, this->id, "odd"));
    }
}

void Keela::CameraManager::stop_recording() {
    camera_stream_even->stop_recording();

    if (split_streams) {
        camera_stream_odd->stop_recording();
    }
}

GstPadProbeReturn Keela::CameraManager::frame_parity_probe_cb(GstPad *pad, GstPadProbeInfo *info, gpointer user_data) {
    GstBuffer *buffer = GST_PAD_PROBE_INFO_BUFFER(info);
    int frame_number = GST_BUFFER_OFFSET(buffer);
    int parity = GPOINTER_TO_INT(user_data);

    if (frame_number % 2 == parity) {
        return GST_PAD_PROBE_OK;  // Pass the frame
    } else {
        return GST_PAD_PROBE_DROP;  // Drop the frame
    }
}

void Keela::CameraManager::set_frame_splitting(bool enabled) {
    split_streams = enabled;
    spdlog::info("Frame splitting {}", enabled ? "enabled" : "disabled");

    if (enabled) {
        if (camera_stream_odd == nullptr) {
            // re-create the odd camera stream if it was previously ejected
            camera_stream_odd = std::make_shared<CameraStreamBin>("camera_stream_odd");
        }

        // Install the probes now that the pipeline is set up
        install_frame_splitting_probes();
        add_odd_camera_stream();
    } else {
        // Remove probes so the even stream gets all frames
        remove_frame_splitting_probes();

        // Eject the odd CameraStreamBin
        camera_stream_odd->PrepareEject();
        camera_stream_odd->Eject(false);  // false = don't send EOS
        camera_stream_odd = nullptr;      // clear the shared_ptr to allow re-creation later
        spdlog::info("Ejected camera_stream_odd from pipeline");
    }
}

void Keela::CameraManager::install_frame_splitting_probes() {
    spdlog::info("Installing frame splitting probes");

    // Install filtering probes on the sink pads of the even and odd tees
    GstPad *even_sink_pad = gst_element_get_static_pad(camera_stream_even->internal_tee, "sink");
    GstPad *odd_sink_pad = gst_element_get_static_pad(camera_stream_odd->internal_tee, "sink");

    if (even_sink_pad) {
        even_frame_probe_id = gst_pad_add_probe(even_sink_pad, GST_PAD_PROBE_TYPE_BUFFER,
                                                frame_parity_probe_cb, GINT_TO_POINTER(EVEN_FRAME), nullptr);
        g_object_unref(even_sink_pad);
        spdlog::info("Installed even frame filter probe");
    }

    if (odd_sink_pad) {
        odd_frame_probe_id = gst_pad_add_probe(odd_sink_pad, GST_PAD_PROBE_TYPE_BUFFER,
                                               frame_parity_probe_cb, GINT_TO_POINTER(ODD_FRAME), nullptr);
        g_object_unref(odd_sink_pad);
        spdlog::info("Installed odd frame filter probe");
    }
}

std::string Keela::CameraManager::get_filename(std::string directory, guint cam_id, std::string suffix) {
    time_t timestamp = std::time(nullptr);
    struct tm datetime = *localtime(&timestamp);
    std::stringstream ss;
    ss << std::put_time(&datetime, "%Y%m%d_%H%M%S_");

    if (suffix != "") {
        suffix = "_" + suffix;
    }

    auto path = std::filesystem::path(directory) / (ss.str() + "cam_" + std::to_string(cam_id) + suffix + ".mkv");

    return path.string();
}

void Keela::CameraManager::add_odd_camera_stream() {
    // @TODO: figure out how to do this more cleanly
    Keela::Bin *camera_stream_bin = camera_stream_odd.get();
    GstElement *camera_stream_elem_odd = static_cast<GstElement *>(*camera_stream_bin);
    gst_object_ref(camera_stream_elem_odd);
    GstBin *this_bin = static_cast<GstBin *>(*this);
    gboolean ret = gst_bin_add(this_bin, camera_stream_elem_odd);
    if (!ret) {
        throw std::runtime_error("Failed to add camera_stream_odd to bin");
    }
    // Sync state with parent if the pipeline is already running
    gboolean sync_result = gst_element_sync_state_with_parent(camera_stream_elem_odd);
    if (!sync_result) {
        spdlog::warn("Failed to sync camera_stream_odd state with parent");
    }
    // Link main tee to camera_stream_odd manually using gst_element_link
    gboolean link_result = gst_element_link(tee_main, camera_stream_elem_odd);
    if (!link_result) {
        throw std::runtime_error("Failed to link tee_main to camera_stream_odd");
    }
}

void Keela::CameraManager::remove_probe_by_id(gulong &probe_id, GstPad *pad, const std::string &probe_name) {
    gst_pad_remove_probe(pad, probe_id);
    g_object_unref(pad);
    probe_id = 0;
    spdlog::info("Removed {} probe", probe_name);
}

void Keela::CameraManager::remove_frame_splitting_probes() {
    if (even_frame_probe_id != 0) {
        GstPad *even_sink_pad = gst_element_get_static_pad(camera_stream_even->internal_tee, "sink");
        remove_probe_by_id(even_frame_probe_id, even_sink_pad, "even frame filter");
    }
    if (odd_frame_probe_id != 0) {
        GstPad *odd_sink_pad = gst_element_get_static_pad(camera_stream_odd->internal_tee, "sink");
        remove_probe_by_id(odd_frame_probe_id, odd_sink_pad, "odd frame filter");
    }
}
