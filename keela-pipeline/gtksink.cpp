//
// Created by brand on 5/31/2025.
//

#include "keela-pipeline/gtksink.h"

#include <stdexcept>
#include <spdlog/spdlog.h>

Keela::GtkSink::GtkSink(): SimpleElement("gtksink") {
}

Keela::GtkSink::GtkSink(const std::string &name): GtkSink() {
    gst_element_set_name(m_element, name.c_str());
}

Keela::GtkSink::~GtkSink() {
}

gpointer Keela::GtkSink::get_widget() {
    GObject* o = nullptr;
    g_object_get(m_element, "widget", &o,nullptr);
    if (!m_element) {
        throw std::runtime_error("Could not get widget");
    }
    spdlog::info("Got widget");
    return o;
}
