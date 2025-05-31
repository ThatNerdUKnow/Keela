//
// Created by brand on 5/26/2025.
//

#ifndef PRESENTATIONBIN_H
#define PRESENTATIONBIN_H
#include <gstreamer-1.0/gst/gst.h>
#include "queuebin.h"

namespace Keela {
    class PresentationBin: public Bin {
        public:
        explicit  PresentationBin(const std::string &name);
        PresentationBin();
        ~PresentationBin();

        void set_framerate(int framerate);
        gpointer get_widget();

    private:
        void init() override;

        void link() override;

        GstElement *videorate = nullptr;
        //GstElement *glsink = nullptr;
        GstElement *sink = nullptr;
    };
}
#endif //PRESENTATIONBIN_H
