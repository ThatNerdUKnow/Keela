//
// Created by brand on 6/30/2025.
//

#include "keela-widgets/GLTraceRender.h"

#include <spdlog/spdlog.h>

Keela::GLTraceRender::GLTraceRender() {
    spdlog::info(__func__);
    gl_area = Gtk::GLArea();
    label = Gtk::Label("Test");
    Container::add(label);
    Container::add(gl_area);
}

Keela::GLTraceRender::~GLTraceRender() {
}
