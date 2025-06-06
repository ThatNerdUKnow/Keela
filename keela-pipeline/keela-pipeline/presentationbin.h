//
// Created by brand on 5/26/2025.
//

#ifndef PRESENTATIONBIN_H
#define PRESENTATIONBIN_H
#include <gstreamer-1.0/gst/gst.h>
#include "queuebin.h"
#include "simpleelement.h"

namespace Keela {
    class PresentationBin : public Bin {
    public:
        explicit PresentationBin(const std::string &name);

        PresentationBin();

        ~PresentationBin();

        void set_framerate(int framerate);

        gpointer get_widget();

    private:
        void init() override;

        void link() override;

        Keela::SimpleElement videorate = Keela::SimpleElement("videorate");
        //GstElement *videorate = nullptr;
        //GstElement *glsink = nullptr;
        //GstElement *sink = nullptr;
        std::shared_ptr<Keela::Element> sink;
    };
}
#endif //PRESENTATIONBIN_H
