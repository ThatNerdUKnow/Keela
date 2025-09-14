//
// Created by brand on 6/3/2025.
//

#include "keela-widgets/cameramanager.h"

#include <keela-pipeline/utils.h>
#include <spdlog/spdlog.h>

#include <sstream>
#include <stdexcept>

#include "keela-pipeline/recordbin.h"
#include "keela-widgets/plugin_utils.h"

Keela::CameraManager::CameraManager(guint id, bool split_streams) : Bin("camera_" + std::to_string(id)),
                                                                    camera(Keela::get_video_test_source_name()) {
    try {
        spdlog::info("Creating camera manager {}", id);
        this->id = id;

        gst_caps_set_simple(base_caps, "format", G_TYPE_STRING, "GRAY8", nullptr);
        g_object_set(caps_filter, "caps", static_cast<GstCaps *>(base_caps), nullptr);

        // Add all elements to the bin
        add_elements(camera, auto_video_convert, caps_filter, transform,
                     tee_main, tee_even, tee_odd,
                     *trace_even, *presentation_even, snapshot_even,
                     *trace_odd, *presentation_odd, snapshot_odd);

        // Link main pipeline: camera -> videoconvert -> capsfilter -> transform -> main_tee
        element_link_many(camera, auto_video_convert, caps_filter, transform, tee_main);

        // Link main tee to both sub-tees
        element_link_many(tee_main, tee_even);
        element_link_many(tee_main, tee_odd);

        // Link even frame outputs
        element_link_many(tee_even, *presentation_even);
        element_link_many(tee_even, snapshot_even);
        element_link_many(tee_even, *trace_even);

        // Link odd frame outputs
        element_link_many(tee_odd, *presentation_odd);
        element_link_many(tee_odd, snapshot_odd);
        element_link_many(tee_odd, *trace_odd);

        // Set up frame splitting if enabled
        this->split_streams = split_streams;
        if (split_streams) {
            install_frame_splitting_probes();
        }

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

void Keela::CameraManager::set_framerate(double framerate) {
    spdlog::info("Setting framerate to {}", framerate);
    int numerator = static_cast<int>(framerate * 10);
    // TODO: caps need to be writable
    base_caps = Caps(static_cast<GstCaps *>(base_caps));
    base_caps.set_framerate(numerator, 10);
    // gst_caps_set_simple(base_caps, "framerate", GST_TYPE_FRACTION, numerator, 10, nullptr);

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
    // Create record bins for both even and odd streams
    std::shared_ptr<RecordBin> record_bin_even = std::make_shared<RecordBin>("recordbin_even");
    std::shared_ptr<RecordBin> record_bin_odd = std::make_shared<RecordBin>("recordbin_odd");

    // Set up file paths for even and odd streams
    std::stringstream ss_even, ss_odd;
    ss_even << this->experiment_directory << "/cam_" << std::to_string(this->id) << "_even.mkv";
    ss_odd << this->experiment_directory << "/cam_" << std::to_string(this->id) << "_odd.mkv";

    record_bin_even->set_directory(ss_even.str());
    record_bin_odd->set_directory(ss_odd.str());

    // Add both record bins to the pipeline
    add_elements(static_cast<GstElement *>(*record_bin_even));
    add_elements(static_cast<GstElement *>(*record_bin_odd));

    // Sync state with parent
    assert(gst_element_sync_state_with_parent(*record_bin_even));
    assert(gst_element_sync_state_with_parent(*record_bin_odd));

    // Link to both tees
    element_link_many(tee_even, static_cast<GstElement *>(*record_bin_even));
    element_link_many(tee_odd, static_cast<GstElement *>(*record_bin_odd));

    // Store both record bins
    record_bins.insert(record_bin_even);
    record_bins.insert(record_bin_odd);
}

void Keela::CameraManager::stop_recording() {
    // notes from example to dynamically remove a bin from a playing pipeline:
    // add a blocking downstream probe to the queue "src" pad
    // inside the blocking callback, add the EOS probe to the last source pad of the bin (it is unclear if this can also be a sink pad)
    // after installing the EOS callback, send an EOS event to the sink pad of the beginning of the bin
    // inside the EOS callback, set the state of the bin to NULL and remove the bin from the pipeline
    for (auto bin: record_bins) {
        bin->PrepareEject();
    }

    for (auto bin: record_bins) {
        bin->Eject(false);
    }
    record_bins.clear();
    spdlog::info("Removed all recordbins from pipeline");
    dump_bin_graph();
}

GstPadProbeReturn Keela::CameraManager::frame_numbering_probe_cb(GstPad *pad, GstPadProbeInfo *info, gpointer user_data) {
    auto *camera_manager = static_cast<CameraManager *>(user_data);
    GstBuffer *buffer = GST_PAD_PROBE_INFO_BUFFER(info);

    // Assign a sequence number to this buffer using offset
    guint64 frame_number = camera_manager->frame_count.fetch_add(1);
    GST_BUFFER_OFFSET(buffer) = frame_number;

    GST_LOG("Frame numbering: assigned number %lu", frame_number);

    return GST_PAD_PROBE_OK;  // Always pass the frame through
}

// @TODO: DRY this up
GstPadProbeReturn Keela::CameraManager::even_frame_probe_cb(GstPad *pad, GstPadProbeInfo *info, gpointer user_data) {
    GstBuffer *buffer = GST_PAD_PROBE_INFO_BUFFER(info);
    guint64 frame_number = GST_BUFFER_OFFSET(buffer);

    if (frame_number % 2 == 0) {
        GST_LOG("Even filter: passing frame %lu", frame_number);
        return GST_PAD_PROBE_OK;  // Pass the frame
    } else {
        GST_LOG("Even filter: dropping frame %lu", frame_number);
        return GST_PAD_PROBE_DROP;  // Drop the frame
    }
}

GstPadProbeReturn Keela::CameraManager::odd_frame_probe_cb(GstPad *pad, GstPadProbeInfo *info, gpointer user_data) {
    GstBuffer *buffer = GST_PAD_PROBE_INFO_BUFFER(info);
    guint64 frame_number = GST_BUFFER_OFFSET(buffer);

    if (frame_number % 2 == 1) {
        GST_LOG("Odd filter: passing frame %lu", frame_number);
        return GST_PAD_PROBE_OK;  // Pass the frame
    } else {
        GST_LOG("Odd filter: dropping frame %lu", frame_number);
        return GST_PAD_PROBE_DROP;  // Drop the frame
    }
}

void Keela::CameraManager::set_frame_splitting(bool enabled) {
    split_streams = enabled;
    spdlog::info("Frame splitting {}", enabled ? "enabled" : "disabled");
    // Reset frame counter when toggling
    frame_count.store(0);

    if (enabled) {
        // Install the probes now that the pipeline is set up
        install_frame_splitting_probes();
    } else {
        // TODO: Remove existing probes when disabled
        // For now, the probes will just pass all frames through
    }
}

void Keela::CameraManager::install_frame_splitting_probes() {
    spdlog::info("Installing frame splitting probes");

    // Install a frame numbering probe before the main tee so we can conditionally drop frames later 
    GstPad *transform_src = gst_element_get_static_pad(transform, "src");
    if (transform_src) {
        gst_pad_add_probe(transform_src, GST_PAD_PROBE_TYPE_BUFFER,
                          frame_numbering_probe_cb, this, nullptr);
        g_object_unref(transform_src);
        spdlog::info("Installed frame numbering probe");
    }

    // Install filtering probes on the sink pads of the even and odd tees
    GstPad *even_sink_pad = gst_element_get_static_pad(tee_even, "sink");
    GstPad *odd_sink_pad = gst_element_get_static_pad(tee_odd, "sink");

    if (even_sink_pad) {
        gst_pad_add_probe(even_sink_pad, GST_PAD_PROBE_TYPE_BUFFER,
                          even_frame_probe_cb, this, nullptr);
        g_object_unref(even_sink_pad);
        spdlog::info("Installed even frame filter probe");
    }

    if (odd_sink_pad) {
        gst_pad_add_probe(odd_sink_pad, GST_PAD_PROBE_TYPE_BUFFER,
                          odd_frame_probe_cb, this, nullptr);
        g_object_unref(odd_sink_pad);
        spdlog::info("Installed odd frame filter probe");
    }
}
