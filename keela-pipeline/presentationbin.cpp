//
// Created by brand on 5/26/2025.
//

#include "keela-pipeline/presentationbin.h"

#include <stdexcept>
#include <spdlog/spdlog.h>

#include <keela-pipeline/gtkglsink.h>
#include <keela-pipeline/gtksink.h>

#include "keela-pipeline/utils.h"


Keela::PresentationBin::PresentationBin(const std::string &name): Bin(name) {
    spdlog::info("{}", __func__);
    PresentationBin::init();
    gboolean ret = false;
    ret = gst_element_set_name(GST_OBJECT(static_cast<GstElement*>(video_rate)), (name+"_videorate").c_str());
    // TODO: name sink
    if (!ret) {
        spdlog::warn("{} Failed to name elements", __func__);
    }
    PresentationBin::link();
}

Keela::PresentationBin::PresentationBin(): Bin() {
    spdlog::info("{}", __func__);
    PresentationBin::init();
    PresentationBin::link();
}

Keela::PresentationBin::~PresentationBin() {
    spdlog::debug(__func__);
}

void Keela::PresentationBin::set_presentation_framerate(const guint framerate) {
    // we do not want to inherit the old caps
    presentation_caps = Caps();
    presentation_caps.set_framerate(60, 1);
    g_object_set(caps_filter, "caps", static_cast<GstCaps *>(presentation_caps), nullptr);
}

gpointer Keela::PresentationBin::get_widget() {
    gpointer obj = nullptr;

    g_object_get(*sink, "widget", &obj, nullptr);
    if (!obj) {
        throw std::runtime_error("No widget found");
    }
    return obj;
}

void Keela::PresentationBin::init() {
    try {
        sink = std::make_shared<GtkGlSink>();
    } catch (const std::exception &e) {
        spdlog::warn("{}", e.what());
        spdlog::info("Falling back to gtksink");
        try {
            sink = std::make_shared<GtkSink>();
        } catch (const std::exception &e2) {
            spdlog::error("{}", e2.what());
            spdlog::warn("falling back to autovideosink");
            sink = std::make_shared<SimpleElement>("autovideosink");
        }
    }
}

void Keela::PresentationBin::link() {
    set_presentation_framerate(60);
    add_elements(video_rate, caps_filter, static_cast<GstElement *>(*sink));
    element_link_many(video_rate, caps_filter, static_cast<GstElement *>(*sink));
    add_ghost_pad(video_rate, "sink");
}
