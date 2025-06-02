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
        std::string message = "Failed to create the element: " + element;
        spdlog::error(message);
        throw std::runtime_error(message);
    }
}

Keela::SimpleElement::SimpleElement(const std::string &element, const std::string &name):SimpleElement(element) {
    spdlog::info("Named {} {}",__func__, name);
    const auto ret = gst_element_set_name(m_element, name.c_str());
    if (!ret) {
        std::string message = "Failed to set name of element";
        spdlog::error(message);
        throw std::runtime_error(message);
    }
}

Keela::SimpleElement::operator struct _GstElement*() const {
    if (m_element) {
        return m_element;
    }else {
        const std::string message = "Internal element is not initialized";
        spdlog::error(message);
        throw std::runtime_error(message);
    }
}

/*
Keela::SimpleElement::operator struct _GstElement*() const {
    if (m_element) {
        return m_element;
    }else {
        const std::string message = "Internal element is not initialized";
        spdlog::error(message);
        throw std::runtime_error(message);
    }
}*/
