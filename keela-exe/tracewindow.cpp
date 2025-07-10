//
// Created by brand on 6/30/2025.
//

#include "tracewindow.h"

Keela::TraceWindow::TraceWindow() {
    set_deletable(false);
    set_default_size(640, 480);
    set_title("Traces");
    Window::add(scrolled_window);
    scrolled_window.add(containter);
}

Keela::TraceWindow::~TraceWindow() {
}

void Keela::TraceWindow::addTrace(std::shared_ptr<Keela::ITraceable> trace) {
    auto widget = std::make_shared<GLTraceRender>(trace);
    traces.push_back(widget);
    containter.add(*widget);
    show_all_children();
}

void Keela::TraceWindow::removeTrace() {
    // TODO:
    auto trace = std::move(traces.back());
    traces.pop_back();
}

int Keela::TraceWindow::num_traces() const {
    return traces.size();
}

void Keela::TraceWindow::set_framerate(double framerate) {
    for (auto trace: traces) {
        trace->set_framerate(framerate);
    }
}
