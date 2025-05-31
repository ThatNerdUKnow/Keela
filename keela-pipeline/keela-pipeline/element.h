//
// Created by brand on 5/30/2025.
//

#ifndef ELEMENT_H
#define ELEMENT_H
#include <string>
#include <gstreamer-1.0/gst/gst.h>

namespace Keela {
    class Element {
    public:
        explicit Element(std::string element);
        Element(std::string element, const std::string &name);

        operator GstElement*() const;
    protected:
        GstElement* m_element;
    };
}
#endif //ELEMENT_H
