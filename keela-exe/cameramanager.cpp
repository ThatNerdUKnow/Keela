//
// Created by brand on 6/3/2025.
//

#include "cameramanager.h"

#include <sstream>
#include <stdexcept>
#include <spdlog/spdlog.h>

#include <keela-pipeline/utils.h>

Keela::CameraManager::CameraManager(guint id, bool split_streams): Bin("camera_" + std::to_string(id)),
                                                                   camera("videotestsrc"),
                                                                   caps_filter("capsfilter"), tee("tee") {
    try {
        spdlog::info("Creating camera manager {}", id);
        this->id = id;

        gst_caps_set_simple(base_caps, "format",G_TYPE_STRING, "GRAY8", nullptr);
        g_object_set(caps_filter, "caps", static_cast<GstCaps *>(base_caps), nullptr);
        add_elements(camera, caps_filter, transform, tee, presentation);
        element_link_many(camera, caps_filter, transform, tee, presentation);

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
}
