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
        ~Bin();

        operator GstBin*() const;
    protected:
        GstBin *bin;
    };
}
#endif //BIN_H
