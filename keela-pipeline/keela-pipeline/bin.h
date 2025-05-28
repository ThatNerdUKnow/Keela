//
// Created by brand on 5/26/2025.
//

#ifndef BIN_H
#define BIN_H
#include <string>
#include <gstreamer-1.0/gst/gst.h>

namespace Keela {
    class Bin {
        public:
        explicit Bin(const std::string &name);
        Bin();

        virtual ~Bin();



        operator GstBin*() const;

        operator GstElement*() const;

    protected:
        GstBin *bin;
    private:
        /* Create required elements and add them to the bin
         */
        virtual void init() {};

        /* Link elements together
         */
        virtual void link() {};
    };
}
#endif //BIN_H
