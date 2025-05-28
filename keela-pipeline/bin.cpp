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
}

Keela::Bin::~Bin() {
    spdlog::debug(__func__);
    g_object_unref(bin);
}

Keela::Bin::operator struct _GstBin*() const {
    const gchar *name = g_type_name(G_OBJECT_TYPE(bin));
    spdlog::debug("{} Type name {}",__func__,name);
    return bin;
}

Keela::Bin::operator struct _GstElement*() const {
    const gchar *name = g_type_name(G_OBJECT_TYPE(bin));
    spdlog::debug("{} Type name {}",__func__,name);
    return GST_ELEMENT(bin);
}
