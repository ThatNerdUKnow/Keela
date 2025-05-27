//
// Created by brand on 5/26/2025.
//

#include "keela-pipeline/presentationbin.h"

#include <stdexcept>
#include <spdlog/spdlog.h>


Keela::PresentationBin::PresentationBin(const std::string &name) {
}

Keela::PresentationBin::PresentationBin() {
    videorate = gst_element_factory_make("videorate",nullptr);
    GstElement *gtkglsink, *videosink;
    assert(videorate != nullptr);
}

Keela::PresentationBin::~PresentationBin() {
    spdlog::debug(__func__);
    g_object_unref(videorate);
    g_object_unref(sink);
}

void Keela::PresentationBin::set_framerate(int framerate) {
}

gpointer Keela::PresentationBin::get_widget() {
    gpointer obj = nullptr;

    g_object_get(sink,"widget", &obj);
    if (!obj) {
        throw std::runtime_error("No widget found");
    }
    return obj;
}
