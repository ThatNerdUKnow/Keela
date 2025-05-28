//
// Created by brand on 5/26/2025.
//

#include "keela-pipeline/recordbin.h"
#include <stdexcept>
#include <spdlog/spdlog.h>
using namespace spdlog;
Keela::RecordBin::RecordBin(const std::string &name): Bin(name) {
    RecordBin::init();
    gboolean ret = false;

    ret = gst_object_set_name(GST_OBJECT(enc), (name + "_enc").c_str());
    ret &= gst_object_set_name(GST_OBJECT(mux),(name + "_mux").c_str());
    ret &= gst_object_set_name(GST_OBJECT(sink),(name + "_sink").c_str());
    if (!ret) {
        throw std::runtime_error("Failed to name Elements");
    }
    RecordBin::link();
}

void Keela::RecordBin::init() {
    enc = gst_element_factory_make("x264enc", nullptr);

    //g_object_set(enc,"quantizer",0);
    // TODO: set pass property of enc to "quant"

    mux = gst_element_factory_make("mp4mux", nullptr);

    // TODO: set file destination of filesink
    sink = gst_element_factory_make("filesink", nullptr);

    if (!enc || !mux || !sink) {
        throw std::runtime_error("Not all elements could be created");
    }
}

void Keela::RecordBin::link() {
    gst_bin_add_many (GST_BIN(bin), enc, mux, sink, nullptr);
    gboolean ret = gst_element_link_many (enc, mux, sink, nullptr);
    if (!ret) {
        throw std::runtime_error("Could not link elements");
    }
}

Keela::RecordBin::RecordBin() {
    RecordBin::init();
    RecordBin::link();
}

Keela::RecordBin::~RecordBin() {
    spdlog::debug(__func__);

    gst_element_unlink_many(enc,mux,sink,nullptr);
    gst_bin_remove_many(*this,enc,mux,sink,nullptr);
    //if (enc) gst_object_unref(enc);
    //if (mux) gst_object_unref(mux);
    //if (sink) gst_object_unref(sink);
}
