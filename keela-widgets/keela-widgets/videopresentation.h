#ifndef VIDEOPRESENTATION_H
#define VIDEOPRESENTATION_H

#include <gtkmm-3.0/gtkmm.h>

#include <memory>

#include "GLCameraRender.h"
#include "tracegizmo.h"

namespace Keela {
    class VideoPresentation : public Gtk::Box {
    public:
        VideoPresentation(const std::string& label_text,
                          std::shared_ptr<Keela::PresentationBin> presentation_bin,
                          bool enable_trace_gizmo = false,
                          int width = 640, int height = 480);
        ~VideoPresentation() override;

        void set_video_size(int width, int height);
        std::shared_ptr<TraceGizmo> get_trace_gizmo() const;

    private:
        std::unique_ptr<GLCameraRender> gl_area;
        std::shared_ptr<TraceGizmo> trace_gizmo;
        Gtk::Label label;
        Gtk::Overlay overlay;
    };
}  // namespace Keela

#endif  // VIDEOPRESENTATION_H