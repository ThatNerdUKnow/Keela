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

Keela::Caps::Caps(GstCaps *c) {
    spdlog::debug("{} creating copy of caps", __func__);
    assert(c != nullptr);
    if (!GST_IS_CAPS(c)) {
        throw std::invalid_argument("caps is not a GstCaps");
    }
    auto copy = gst_caps_copy(c);
    if (!copy) {
        throw std::invalid_argument("failed to copy caps");
    }
    m_caps = std::shared_ptr<GstCaps>(copy, delete_caps);
}

Keela::Caps::~Caps() {
    spdlog::debug(__func__);
}

Keela::Caps::operator struct _GstCaps *() const {
    return m_caps.get();
}
