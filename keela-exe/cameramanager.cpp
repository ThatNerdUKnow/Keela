//
// Created by brand on 6/3/2025.
//

#include "cameramanager.h"

#include <sstream>
#include <stdexcept>
#include <spdlog/spdlog.h>

#include <keela-pipeline/utils.h>

Keela::CameraManager::CameraManager(guint id, bool split_streams): camera("videotestsrc"), caps_filter("capsfilter"),
                                                                   tee("tee"), Bin("camerabin_" + id) {
    try {
        spdlog::info("Creating camera manager " + id);
        this->id = id;
        GstElement *bin = *this;
        std::string name = "camerabin_" + id;
        add_elements(camera, caps_filter, transform, tee, presentation);
        element_link_many(camera, caps_filter, transform, tee, presentation);

        spdlog::info("Created camera manager " + id);
    } catch (const std::exception &e) {
        std::stringstream ss;

        ss << "camera" << id << ".dot";
        spdlog::error("could not create camera manager. attempting to dump bin to {}", ss.str());
        gst_debug_bin_to_dot_file(bin.get(), GST_DEBUG_GRAPH_SHOW_ALL, ss.str().c_str());
        throw;
    }
}

Keela::CameraManager::~CameraManager() {
}

void Keela::CameraManager::set_framerate(double framerate) {
}
