#ifndef CAMERASTREAMBIN_H
#define CAMERASTREAMBIN_H


#include "EjectableElement.h"
#include "caps.h"
#include "queuebin.h"
#include "simpleelement.h"


namespace Keela {
    class CameraStreamBin final : public QueueBin, public EjectableElement {
    public:
        explicit CameraStreamBin(const std::string &name);

        CameraStreamBin();

        ~CameraStreamBin() override;

        // Keela::SimpleElement sink = SimpleElement("appsink");

    private:
        void init() override;

        void link() override;

        /// Used to skip frames for the purposes of presentation
        // Keela::SimpleElement video_rate = SimpleElement("videorate");
        /// Controls the target presentation framerate
        // Keela::SimpleElement caps_filter = SimpleElement("capsfilter");
        // Keela::Caps presentation_caps = Caps();
    };
}  // namespace Keela
#endif  // CAMERASTREAMBIN_H
