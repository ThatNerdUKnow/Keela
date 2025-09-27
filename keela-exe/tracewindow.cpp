//
// Created by brand on 6/30/2025.
//

#include "tracewindow.h"

Keela::TraceWindow::TraceWindow() {
    set_deletable(false);
    set_default_size(640, 480);
    set_title("Traces");
    Window::add(scrolled_window);
    scrolled_window.add(container);
    Glib::signal_timeout().connect(sigc::mem_fun(this, &TraceWindow::on_timeout), 1000 / 60);
}

Keela::TraceWindow::~TraceWindow() {
}

void Keela::TraceWindow::addTrace(std::shared_ptr<Keela::ITraceable> trace) {
    auto widget = std::make_shared<GLTraceRender>(trace);
    traces.push_back(widget);
    container.add(*widget);
    show_all_children();
    spdlog::info("TraceWindow::{}: Trace added", __func__);
}

void Keela::TraceWindow::removeTrace() {
    // TODO:
    auto trace = std::move(traces.back());
    traces.pop_back();
}

int Keela::TraceWindow::num_traces() const {
    return traces.size();
}

bool Keela::TraceWindow::on_timeout() {
    for (const auto &trace: traces) {
        trace->queue_draw();
    }
    return true;
}
