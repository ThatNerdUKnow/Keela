//
// Created by brand on 6/30/2025.
//

#include "tracewindow.h"

Keela::TraceWindow::TraceWindow() {
    set_deletable(false);
    set_default_size(640, 480);
    set_title("Traces");
    Window::add(containter);
}

Keela::TraceWindow::~TraceWindow() {
}

void Keela::TraceWindow::addTrace() {
    auto trace = std::make_shared<GLTraceRender>();
    traces.push_back(trace);
    containter.add(*trace);
    show_all_children();
}

void Keela::TraceWindow::removeTrace() {
    // TODO:
    auto trace = std::move(traces.back());
    traces.pop_back();
}

int Keela::TraceWindow::num_traces() {
    return traces.size();
}
