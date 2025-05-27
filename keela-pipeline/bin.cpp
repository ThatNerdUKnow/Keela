//
// Created by brand on 5/26/2025.
//

#include "keela-pipeline/bin.h"

#include <stdexcept>

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
    g_object_unref(bin);
}

Keela::Bin::operator struct _GstBin*() const {
    return bin;
}
