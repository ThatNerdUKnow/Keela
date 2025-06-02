//
// Created by brand on 5/26/2025.
//

#include "keela-pipeline/transformbin.h"

#include <stdexcept>
#include <spdlog/spdlog.h>

#include "keela-pipeline/gst-helpers.h"

Keela::TransformBin::TransformBin():Bin() {
    spdlog::info("{}",__func__);
    TransformBin::init();
    TransformBin::link();
}

Keela::TransformBin::TransformBin(const std::string &name):Bin(name) {
    spdlog::info("{}",__func__);
    TransformBin::init();
    gboolean ret = false;
    ret = gst_object_set_name(GST_OBJECT(scale), (name + "_scale").c_str());
    if (!ret) {
        spdlog::warn("{} Failed to name elements",__func__);
    }
    TransformBin::link();
}

Keela::TransformBin::~TransformBin() {
    spdlog::debug(__func__);
}

void Keela::TransformBin::init() {
    scale = gst_element_factory_make("videoscale", nullptr);
    if (!scale) {
        throw std::runtime_error("Failed to initialize elements");
    }
    auto variant = Keela::gst_enum_variant_by_nick(G_OBJECT(scale),"method","bilinear");
    g_object_set(scale,"method",variant,nullptr);
    gst_bin_add(*this,scale);
}

void Keela::TransformBin::link() {
    gst_bin_add_many(*this,scale,nullptr);
    add_ghost_pad(scale,"sink");
    add_ghost_pad(scale,"src");
}
