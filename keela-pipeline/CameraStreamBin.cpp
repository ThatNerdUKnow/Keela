#include "keela-pipeline/CameraStreamBin.h"

#include <keela-pipeline/gtkglsink.h>
#include <keela-pipeline/gtksink.h>
#include <spdlog/spdlog.h>

#include <stdexcept>

#include "keela-pipeline/utils.h"

Keela::CameraStreamBin::CameraStreamBin(const std::string &name) : QueueBin(name) {
    spdlog::info("{}", __func__);
    CameraStreamBin::init();

    CameraStreamBin::link();
}

Keela::CameraStreamBin::CameraStreamBin() : QueueBin() {
    spdlog::info("{}", __func__);
    CameraStreamBin::init();
    CameraStreamBin::link();
}

Keela::CameraStreamBin::~CameraStreamBin() {
    spdlog::debug(__func__);
}

void Keela::CameraStreamBin::init() {
    // g_object_set(sink, "max-buffers", 1, nullptr);
    // g_object_set(sink, "drop", true, nullptr);
}

void Keela::CameraStreamBin::link() {
    // set_presentation_framerate(60);
    // add_elements(video_rate, caps_filter, sink);
    // element_link_many(video_rate, caps_filter, sink);
    // link_queue(video_rate);
    // add_ghost_pad(video_rate, "sink");
}
