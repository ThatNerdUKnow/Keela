//
// Created by brand on 6/30/2025.
//

#include "tracewindow.h"

Keela::TraceWindow::TraceWindow() {
}

Keela::TraceWindow::~TraceWindow() {
}

void Keela::TraceWindow::addTrace() {
    auto trace = std::make_shared<GLTraceRender>();
    traces.push_back(trace);
    containter.add(*trace);
}

void Keela::TraceWindow::removeTrace() {
    // TODO:
}
