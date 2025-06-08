//
// Created by brand on 6/7/2025.
//

#include "keela-pipeline/caps.h"

#include <spdlog/spdlog.h>

#include "keela-pipeline/utils.h"

Keela::Caps::Caps() {
    auto caps = gst_caps_new_simple("video/x-raw", nullptr);
    m_caps = std::shared_ptr<GstCaps>(caps, delete_caps);
}

Keela::Caps::~Caps() {
    spdlog::debug(__func__);
}

Keela::Caps::operator struct _GstCaps *() const {
    return m_caps.get();
}
