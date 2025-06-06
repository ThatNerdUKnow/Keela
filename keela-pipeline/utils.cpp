//
// Created by brand on 6/5/2025.
//
#include "keela-pipeline/utils.h"

void Keela::delete_bin(GstBin *bin) {
    auto refcount = GST_OBJECT_REFCOUNT(bin);
    auto name = GST_OBJECT_NAME(bin);
    spdlog::trace("{} {} refcount {}", __func__, name, refcount);
    auto parent = gst_element_get_parent(GST_ELEMENT(bin));
    if (!parent) {
        // if bin does not have parent, we are responsible for cleaning it up
        assert(refcount == 1);
        g_object_unref(bin);
    } else {
        // otherwise the bin will be deleted once its parent is deleted
        gst_object_unref(parent);
    }
}

void Keela::delete_element(GstElement *e) {
    auto refcount = GST_OBJECT_REFCOUNT(e);
    auto name = GST_OBJECT_NAME(e);
    spdlog::trace("{} {} refcount {}", __func__, name, refcount);
    auto parent = gst_element_get_parent(GST_ELEMENT(e));
    if (!parent) {
        // if element does not have parent, we are responsible for cleaning it up
        assert(refcount == 1);
        g_object_unref(e);
    } else {
        // otherwise the element will be deleted once its parent is deleted
        gst_object_unref(parent);
    }
}
