//
// Created by brand on 5/26/2025.
//

#include "keela-pipeline/presentationbin.h"

#include <stdexcept>
#include <spdlog/spdlog.h>


Keela::PresentationBin::PresentationBin(const std::string &name):Bin(name) {
    PresentationBin::init();
    gboolean ret = false;
    ret = gst_element_set_name(GST_OBJECT(videorate), (name+"_videorate").c_str());
    if (glsink) {
        ret |= gst_element_set_name(GST_OBJECT(glsink), name.c_str());
    }
    if (!ret) {
        spdlog::warn("{} Failed to name elements",__func__);
    }
    PresentationBin::link();
}

Keela::PresentationBin::PresentationBin():Bin() {
    PresentationBin::init();
    PresentationBin::link();

}

Keela::PresentationBin::~PresentationBin() {
    spdlog::debug(__func__);
}

void Keela::PresentationBin::set_framerate(int framerate) {
    // TODO:
}

gpointer Keela::PresentationBin::get_widget() {
    gpointer obj = nullptr;

    g_object_get(sink,"widget", &obj, nullptr);
    if (!obj) {
        throw std::runtime_error("No widget found");
    }
    return obj;
}

void Keela::PresentationBin::init() {
    // my spidey sense tells me this leaks
    videorate = gst_element_factory_make("videorate",nullptr);
    glsink = gst_element_factory_make("glsinkbin",nullptr);
    sink = gst_element_factory_make("gtkglsink",nullptr);
    if (glsink && sink) {
        spdlog::info("Successfully created gtk opengl elements");
        g_object_set(glsink, "sink",sink,nullptr);
    } else {
        spdlog::warn("Failed to create gtk opengl elements");
        spdlog::info("Falling back to gtksink");
        sink = gst_element_factory_make("gtksink",nullptr);
        if (!sink) {
            const std::string msg = "Failed to initialize presentation bin";
            spdlog::error(msg);
            throw std::runtime_error(msg);
        }
    }
}

void Keela::PresentationBin::link() {

    if (glsink) {
        gst_bin_add_many(*this,videorate,glsink,sink,nullptr);
    } else {
        gst_bin_add_many(*this,videorate,sink,nullptr);
    }
}
