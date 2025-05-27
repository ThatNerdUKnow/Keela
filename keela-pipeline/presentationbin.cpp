//
// Created by brand on 5/26/2025.
//

#include "keela-pipeline/presentationbin.h"

#include <stdexcept>

Keela::PresentationBin::PresentationBin(int framerate, const std::string &name) {
    auto cname = name.c_str();
    videorate = gst_element_factory_make("videorate",cname);


}

Keela::PresentationBin::~PresentationBin() {
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
