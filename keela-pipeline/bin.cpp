//
// Created by brand on 5/26/2025.
//

#include "keela-pipeline/bin.h"

#include <stdexcept>
#include <spdlog/spdlog.h>
Keela::Bin::Bin(const std::string &name): Bin() {
    gst_object_set_name(GST_OBJECT(bin),name.c_str());
}

Keela::Bin::Bin() {
    bin = GST_BIN(gst_bin_new(nullptr));
    if (bin == nullptr) {
        throw std::runtime_error("Failed to create bin");
    }

    if (!gst_element_add_pad(*this,gst_ghost_pad_new_no_target("src",GST_PAD_SRC))) {
        throw std::runtime_error("Failed to add source ghost pad");
    }

    if (!gst_element_add_pad(*this,gst_ghost_pad_new_no_target("sink",GST_PAD_SINK))) {
        throw std::runtime_error("Failed to add sink ghost pad");
    }
}

Keela::Bin::~Bin() {
    spdlog::debug(__func__);
    g_object_unref(bin);
}

Keela::Bin::operator struct _GstBin*() const {
    const gchar *name = g_type_name(G_OBJECT_TYPE(bin));
    spdlog::trace("{} Type name {}",__func__,name);
    return bin;
}

Keela::Bin::operator struct _GstElement*() const {
    const gchar *name = g_type_name(G_OBJECT_TYPE(bin));
    spdlog::trace("{} Type name {}",__func__,name);
    return GST_ELEMENT(bin);
}

void Keela::Bin::add_ghost_pad(GstElement *element, const std::string &pad_name) const{
    spdlog::info("{} {}",__func__,pad_name);
    GstObject* parent = gst_object_get_parent(GST_OBJECT(element));

    if (GST_BIN(parent) != *this) {
        throw std::runtime_error("Element is not owned by this bin");
    }
    gst_object_unref(GST_OBJECT(parent));

    GstPad* target = gst_element_get_static_pad(element, pad_name.c_str());
    if (!target) {
        throw std::runtime_error("Failed to get target pad");
    }
    GstPad* ghost =  gst_ghost_pad_new(nullptr, target);
    if (!ghost) {
        throw std::runtime_error("Failed to create ghost pad");
    }
    gst_object_unref(target);
    gst_element_add_pad(*this,ghost);
}
