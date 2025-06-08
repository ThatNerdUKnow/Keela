//
// Created by brand on 5/26/2025.
//

#ifndef PRESENTATIONBIN_H
#define PRESENTATIONBIN_H
#include <gstreamer-1.0/gst/gst.h>

#include "caps.h"
#include "queuebin.h"
#include "simpleelement.h"

namespace Keela {
    class PresentationBin : public Bin {
    public:
        explicit PresentationBin(const std::string &name);

        PresentationBin();

        ~PresentationBin() override;

        void set_presentation_framerate(guint framerate);

        gpointer get_widget();

    private:
        void init() override;

        void link() override;

        /// Used to skip frames for the purposes of presentation
        Keela::SimpleElement video_rate = SimpleElement("videorate");
        /// Controls the target presentation framerate
        Keela::SimpleElement caps_filter = SimpleElement("capsfilter");
        Keela::Caps presentation_caps = Caps();
        std::shared_ptr<Keela::Element> sink;
    };
}
#endif //PRESENTATIONBIN_H
