//
// Created by brand on 6/25/2025.
//

#include "keela-widgets/tracegizmo.h"

Keela::TraceGizmo::TraceGizmo() {
}

Keela::TraceGizmo::~TraceGizmo() {
}

bool Keela::TraceGizmo::on_button_press_event(GdkEventButton *button_event) {
    return DrawingArea::on_button_press_event(button_event);
}

bool Keela::TraceGizmo::on_button_release_event(GdkEventButton *release_event) {
    return DrawingArea::on_button_release_event(release_event);
}

bool Keela::TraceGizmo::on_motion_notify_event(GdkEventMotion *motion_event) {
    return DrawingArea::on_motion_notify_event(motion_event);
}

bool Keela::TraceGizmo::on_draw(const Cairo::RefPtr<Cairo::Context> &cr) {
    cr->save();
    cr->set_source_rgba(255, 0, 0, 1.0);
    cr->set_line_width(2);
    cr->rectangle(50, 50, 100, 100);
    cr->stroke();
    cr->restore();
    return DrawingArea::on_draw(cr);
}
