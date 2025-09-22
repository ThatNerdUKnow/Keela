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
}

Keela::TraceWindow::~TraceWindow() {
}

void Keela::TraceWindow::addTraces(const std::vector<std::shared_ptr<Keela::ITraceable>>& traces_to_add) {
    if (traces_to_add.empty()) {
        return;
    }
    
    // Create a horizontal box to hold traces from this camera
    auto row_box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL);
    row_box->set_spacing(10);
    
    for (const auto& trace : traces_to_add) {
        auto widget = std::make_shared<GLTraceRender>(trace);
        traces.push_back(widget);
        row_box->pack_start(*widget, true, true, 0);
    }
    
    container.pack_start(*row_box, false, false, 10);
    show_all_children();
    spdlog::info("TraceWindow::{}: Added {} traces in a row", __func__, traces_to_add.size());
}

void Keela::TraceWindow::removeTrace() {
    // TODO:
    auto trace = std::move(traces.back());
    traces.pop_back();
}

int Keela::TraceWindow::num_traces() const {
    return traces.size();
}
