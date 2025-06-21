//
// Created by brand on 6/3/2025.
//

#include "cameramanager.h"

#include <sstream>
#include <stdexcept>
#include <spdlog/spdlog.h>

#include <keela-pipeline/utils.h>

#include "keela-pipeline/recordbin.h"

Keela::CameraManager::CameraManager(guint id, bool split_streams): Bin("camera_" + std::to_string(id)),
                                                                   camera("videotestsrc") {
    try {
        spdlog::info("Creating camera manager {}", id);
        this->id = id;

        gst_caps_set_simple(base_caps, "format",G_TYPE_STRING, "GRAY8", nullptr);
        g_object_set(caps_filter, "caps", static_cast<GstCaps *>(base_caps), nullptr);
        add_elements(camera, auto_video_convert, caps_filter, transform, tee, *presentation, snapshot);
        element_link_many(camera, auto_video_convert, caps_filter, transform, tee, *presentation);
        element_link_many(tee, snapshot);

        spdlog::info("Created camera manager {}", id);
    } catch (const std::exception &e) {
        std::stringstream ss;

        ss << "camera" << id << ".dot";
        spdlog::error("could not create camera manager. attempting to dump bin to {}", ss.str());
        gst_debug_bin_to_dot_file(bin.get(), GST_DEBUG_GRAPH_SHOW_ALL, ss.str().c_str());
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
    //gst_caps_set_simple(base_caps, "framerate", GST_TYPE_FRACTION, numerator, 10, nullptr);

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
    std::shared_ptr<RecordBin> record_bin = std::make_shared<RecordBin>();
    std::stringstream ss;
    ss << this->experiment_directory << "\\cam_" << std::to_string(this->id) << "(%d).mkv";
    record_bin->set_directory(ss.str());
    add_elements(*record_bin);
    assert(gst_element_sync_state_with_parent(*record_bin));
    element_link_many(tee, *record_bin);
    record_bins.insert(record_bin);
}

void Keela::CameraManager::stop_recording() {
    for (auto bin: record_bins) {
        gst_element_get_static_pad(*bin, "sink");
        record_bins.erase(bin);
    }
}

GstPadProbeReturn Keela::CameraManager::pad_block_callback(GstPad *pad, GstPadProbeInfo *info, gpointer user_data) {
    auto recordbin = static_cast<std::shared_ptr<RecordBin> *>(user_data);
    gst_pad_remove_probe(pad, GST_PAD_PROBE_INFO_ID(info));

    auto file_sink_pad = gst_element_get_static_pad((*recordbin)->sink, "sink");
    auto copy_recordbin = new std::shared_ptr<RecordBin>(*recordbin);
    gst_pad_add_probe(file_sink_pad, GST_PAD_PROBE_TYPE_BLOCK_DOWNSTREAM, event_callback, copy_recordbin, nullptr);
    g_object_unref(file_sink_pad);
    delete recordbin;
    return GST_PAD_PROBE_OK;
}

GstPadProbeReturn Keela::CameraManager::event_callback(GstPad *pad, GstPadProbeInfo *info, gpointer user_data) {
    auto recordbin = static_cast<std::shared_ptr<RecordBin> *>(user_data);
    if (GST_EVENT_TYPE(GST_PAD_PROBE_INFO_DATA(info)) != GST_EVENT_EOS) {
        return GST_PAD_PROBE_OK;
    }

    gst_pad_remove_probe(pad, GST_PAD_PROBE_INFO_ID(info));

    gst_element_set_state(**recordbin, GST_STATE_NULL);
    auto parent = GST_ELEMENT_PARENT(static_cast<GstElement*>(**recordbin));
    // because recordbin is supposed to be inside a pipeline, it should have a parent
    assert(parent != nullptr);
    gst_bin_remove(GST_BIN(parent), **recordbin);
    delete recordbin;
    return GST_PAD_PROBE_DROP;
}
