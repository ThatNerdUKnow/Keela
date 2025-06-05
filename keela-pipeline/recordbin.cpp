//
// Created by brand on 5/26/2025.
//

#include "keela-pipeline/recordbin.h"
#include <stdexcept>
#include <spdlog/spdlog.h>
#include "keela-pipeline/gst-helpers.h"
#include "keela-pipeline/utils.h"

using namespace spdlog;

Keela::RecordBin::RecordBin(const std::string &name): QueueBin(name) {
    spdlog::info("{}", __func__);
    gboolean ret = false;

    ret = gst_object_set_name(GST_OBJECT(static_cast<GstElement*>(this->enc)), (name + "_enc").c_str());
    ret &= gst_object_set_name(GST_OBJECT(static_cast<GstElement*>(mux)), (name + "_mux").c_str());
    ret &= gst_object_set_name(GST_OBJECT(static_cast<GstElement*>(sink)), (name + "_sink").c_str());
    if (!ret) {
        throw std::runtime_error("Failed to name Elements");
    }
    RecordBin::link();
}

void Keela::RecordBin::link() {
    add_elements(enc, mux, sink);
    element_link_many(enc, mux, sink);
    link_queue(enc);
}

Keela::RecordBin::RecordBin(): QueueBin() {
    spdlog::info("{}", __func__);
    RecordBin::link();
}

Keela::RecordBin::~RecordBin() {
    spdlog::debug(__func__);
}
