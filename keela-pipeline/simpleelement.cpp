//
// Created by brand on 5/30/2025.
//

#include "./keela-pipeline/simpleelement.h"

#include <stdexcept>
#include <utility>
#include <spdlog/spdlog.h>

Keela::SimpleElement::SimpleElement(const std::string &element) {
    spdlog::info("{} {}",__func__, element);
    m_element = gst_element_factory_make(element.c_str(), nullptr);
    if (!m_element) {
        throw std::runtime_error("Failed to create element: " + element);
    }
}

Keela::SimpleElement::SimpleElement(const std::string &element, const std::string &name):SimpleElement(element) {
    spdlog::info("Named {} {}",__func__, name);
    const auto ret = gst_element_set_name(m_element, name.c_str());
    if (!ret) {
        throw std::runtime_error("Failed to set name of element");
    }
}

Keela::SimpleElement::operator struct _GstElement*() const {
    if (m_element) {
        const gchar *tname = g_type_name(G_OBJECT_TYPE(m_element));
        const gchar* name = gst_element_get_name(m_element);
        if (!name) {
            spdlog::trace("{} Type name {}",__func__, tname);
        } else {
            spdlog::trace("{}::{} Type name {}",name,__func__, tname);
        }
        return m_element;
    }else {
        const std::string message = "Internal element is not initialized";
        spdlog::error(message);
        throw std::runtime_error(message);
    }
}
