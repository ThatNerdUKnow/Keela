//
// Created by brand on 5/26/2025.
//

#include "keela-pipeline/TransformBin.h"

#include <stdexcept>
#include <spdlog/spdlog.h>

Keela::TransformBin::TransformBin() {
    TransformBin::init();
    TransformBin::link();
}

Keela::TransformBin::TransformBin(const std::string &name):Bin(name) {
    TransformBin::init();
    gboolean ret = false;
    ret = gst_object_set_name(GST_OBJECT(scale), (name + "_scale").c_str());
    if (!ret) {
        throw std::runtime_error("Failed to name Elements");
    }
    TransformBin::link();
}

Keela::TransformBin::~TransformBin() {
    spdlog::debug(__func__);
    gst_bin_remove_many(*this,scale, nullptr);
    //if (scale) g_object_unref(scale);
}

void Keela::TransformBin::init() {
    scale = gst_element_factory_make("videoscale", nullptr);
}

void Keela::TransformBin::link() {
    gst_bin_add_many(*this,scale,nullptr);
}
