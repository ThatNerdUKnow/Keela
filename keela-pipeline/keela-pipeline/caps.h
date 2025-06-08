//
// Created by brand on 6/7/2025.
//

#ifndef CAPS_H
#define CAPS_H
#include <memory>
#include <gstreamer-1.0/gst/gst.h>

namespace Keela {
    class Caps {
    public:
        Caps();

        ~Caps();

        operator GstCaps *() const;

    private:
        std::shared_ptr<GstCaps> m_caps;
    };
}
#endif //CAPS_H
