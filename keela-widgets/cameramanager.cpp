//
// Created by brand on 6/3/2025.
//

#include "keela-widgets/cameramanager.h"

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
        add_elements(camera, auto_video_convert, caps_filter, transform, tee, *trace, *presentation, snapshot);
        element_link_many(camera, auto_video_convert, caps_filter, transform, tee, *presentation);
        element_link_many(tee, snapshot);
        element_link_many(tee, *trace);

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
    std::shared_ptr<RecordBin> record_bin = std::make_shared<RecordBin>("recordbin");
    std::stringstream ss;
    ss << this->experiment_directory << "\\cam_" << std::to_string(this->id) << ".mkv";
    record_bin->set_directory(ss.str());
    add_elements(static_cast<GstElement *>(*record_bin));
    assert(gst_element_sync_state_with_parent(*record_bin));
    element_link_many(tee, static_cast<GstElement *>(*record_bin));
    record_bins.insert(record_bin);
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
