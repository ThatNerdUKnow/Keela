//
// Created by brand on 6/5/2025.
//
#include "keela-pipeline/utils.h"

void Keela::delete_bin(GstBin *bin) {
    auto refcount = GST_OBJECT_REFCOUNT(bin);
    auto name = GST_OBJECT_NAME(bin);
    spdlog::debug("{} {} refcount {}", __func__, name, refcount);
    if (refcount > 0)
    g_object_unref(bin);
}

void Keela::delete_element(GstElement *e) {
    auto refcount = GST_OBJECT_REFCOUNT(e);
    auto name = GST_OBJECT_NAME(e);
    spdlog::debug("{} {} refcount {}", __func__, name, refcount);
    if (refcount > 0)
    g_object_unref(e);
}
