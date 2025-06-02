//
// Created by brand on 5/30/2025.
//

#ifndef ELEMENT_H
#define ELEMENT_H
#include <string>
#include <gstreamer-1.0/gst/gst.h>
#include "elementbase.h"

namespace Keela {

    class SimpleElement: public Keela::Element {
    public:
        explicit SimpleElement(const std::string &element);
        SimpleElement(const std::string &element, const std::string &name);

        operator GstElement*() const override;

        //operator GstElement*() const;
    protected:
        GstElement* m_element;
    };
}
#endif //ELEMENT_H
