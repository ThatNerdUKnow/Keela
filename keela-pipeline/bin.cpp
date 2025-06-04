//
// Created by brand on 5/26/2025.
//

#include "keela-pipeline/bin.h"

#include <stdexcept>
#include <spdlog/spdlog.h>
Keela::Bin::Bin(const std::string &name): Bin() {
    spdlog::info("Naming bin {}",name);
    if (!gst_object_set_name(GST_OBJECT(bin),name.c_str())) {
        spdlog::warn("Could not set name of bin");
    }
}

// apparently needed when using bin in gst functions
Keela::Bin::Bin(const Bin &bin) {
    GstBin* b = bin;
    auto name = gst_element_get_name(b);
    assert(name != nullptr);
    const auto other = gst_object_ref(bin.bin);
    auto refcount = GST_OBJECT_REFCOUNT(other);
    spdlog::trace("{} copy constructor: Increased refcount of {} to {}",__func__,name,refcount);
    g_free(name);
    this->bin = GST_BIN(other);
}

Keela::Bin::Bin() {
    spdlog::info("{} {}",typeid(*this).name(),__func__);
    bin = GST_BIN(gst_bin_new(nullptr));
    if (bin == nullptr) {
        throw std::runtime_error("Failed to create bin");
    }
}

Keela::Bin::~Bin() {
    auto refcount = GST_OBJECT_REFCOUNT(bin);
    spdlog::trace("{} refcount {}",__func__,refcount);
    auto parent = gst_element_get_parent(GST_ELEMENT(bin));
    if (!parent) {
        g_object_unref(bin);
    }
}

Keela::Bin::operator struct _GstElement*() const {
    const gchar *tname = g_type_name(G_OBJECT_TYPE(bin));
    const gchar *name = gst_element_get_name(GST_ELEMENT(bin));
    if (!name) {
        spdlog::trace("{} Type name {}",__func__,tname);
    } else {
        spdlog::trace("{}::{} Type name {}",name,__func__,tname);
    }
    return GST_ELEMENT(bin);
}

Keela::Bin::operator struct _GstBin*() const {
    const gchar *tname = g_type_name(G_OBJECT_TYPE(bin));
    const gchar *name = gst_element_get_name(GST_ELEMENT(bin));
    if (!name) {
        spdlog::trace("{} Type name {}",__func__,tname);
    }else {
        spdlog::trace("{}::{} Type name {}",name,__func__,tname);
    }
    return bin;
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
