//
// Created by brand on 5/26/2025.
//

#include "keela-pipeline/commonbin.h"

#include <stdexcept>
#include <spdlog/spdlog.h>

Keela::CommonBin::CommonBin() {
    scale = gst_element_factory_make("videoscale", nullptr);
    tee = gst_element_factory_make("tee", nullptr);
}

Keela::CommonBin::CommonBin(const std::string &name):CommonBin() {
    gboolean ret = false;
    ret = gst_object_set_name(GST_OBJECT(scale), (name + "_scale").c_str());
    ret &= gst_object_set_name(GST_OBJECT(tee), (name + "_tee").c_str());
    if (!ret) {
        throw std::runtime_error("Failed to name Elements");
    }
}

Keela::CommonBin::~CommonBin() {
    spdlog::debug(__func__);
    g_object_unref(scale);
    g_object_unref(tee);
}
