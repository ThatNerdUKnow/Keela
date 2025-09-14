#include "keela-widgets/videopresentation.h"

Keela::VideoPresentation::VideoPresentation(const std::string& label_text,
                                            std::shared_ptr<Keela::PresentationBin> presentation_bin,
                                            bool enable_trace_gizmo,
                                            int width, int height)
    : Gtk::Box(Gtk::ORIENTATION_VERTICAL), gl_area(std::make_unique<GLCameraRender>(presentation_bin)), label(label_text) {
    set_spacing(5);
    set_border_width(5);

    gl_area->set_size_request(width, height);

    overlay.add(*gl_area);

    if (enable_trace_gizmo) {
        trace_gizmo = std::make_shared<TraceGizmo>();
        overlay.add_overlay(*trace_gizmo);
    }

    pack_start(label, false, false, 5);
    pack_start(overlay, false, false, 5);

    show_all_children();
}

Keela::VideoPresentation::~VideoPresentation() = default;

void Keela::VideoPresentation::set_video_size(int width, int height) {
    gl_area->set_size_request(width, height);
}

std::shared_ptr<Keela::TraceGizmo> Keela::VideoPresentation::get_trace_gizmo() const {
    return trace_gizmo;
}
