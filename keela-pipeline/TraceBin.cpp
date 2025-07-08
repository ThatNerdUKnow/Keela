//
// Created by brand on 7/7/2025.
//

#include "keela-pipeline/gst-helpers.h"
#include "keela-pipeline/tracebin.h"

#include "keela-pipeline/utils.h"

Keela::TraceBin::TraceBin(): QueueBin("TraceBin") {
    TraceBin::init();
    TraceBin::link();
}

Keela::TraceBin::TraceBin(std::string name): QueueBin(name) {
    TraceBin::init();
    TraceBin::link();
}

Keela::TraceBin::~TraceBin() {
}

void Keela::TraceBin::enable_trace(bool do_enable) {
    auto max_buffers = do_enable ? 0 : 1;
    g_object_set(sink, "max-buffers", max_buffers, nullptr);

    g_object_set(sink, "drop", !do_enable, nullptr);
}

void Keela::TraceBin::init() {
    add_elements(sink);
    enable_trace(false);
}

void Keela::TraceBin::link() {
    link_queue(sink);
}
