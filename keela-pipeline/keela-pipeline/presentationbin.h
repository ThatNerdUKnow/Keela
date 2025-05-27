//
// Created by brand on 5/26/2025.
//

#ifndef PRESENTATIONBIN_H
#define PRESENTATIONBIN_H
#include <gstreamer-1.0/gst/gst.h>

#include "bin.h"

namespace Keela {
    class PresentationBin: Keela::Bin {
        public:
        explicit  PresentationBin(int framerate = 60);
        ~PresentationBin();

        void set_framerate(int framerate);
        private:
        GstElement *videorate;
        GstElement *sink;
    };
}
#endif //PRESENTATIONBIN_H
