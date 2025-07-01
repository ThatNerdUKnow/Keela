//
// Created by brand on 6/30/2025.
//

#include "keela-widgets/GLTraceRender.h"

#include <spdlog/spdlog.h>

Keela::GLTraceRender::GLTraceRender(const std::shared_ptr<ITraceable> &cam_to_trace): Gtk::Box(
    Gtk::ORIENTATION_VERTICAL) {
    spdlog::info(__func__);
    gl_area = Gtk::GLArea();
    label = Gtk::Label(cam_to_trace->get_name());
    Container::add(label);
    Container::add(gl_area);
    gl_area.set_size_request(300, 128);
    trace = cam_to_trace;
}

Keela::GLTraceRender::~GLTraceRender() = default;
