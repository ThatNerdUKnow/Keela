//
// Created by brand on 6/25/2025.
//

#include "keela-widgets/tracegizmo.h"

#include <spdlog/spdlog.h>

Keela::TraceGizmo::TraceGizmo() {
    add_events(Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::BUTTON_PRESS_MASK);
    //center = Gdk::Point(250, 250);
    bounds = Gdk::Rectangle(500, 500, 100, 200);
}

Keela::TraceGizmo::~TraceGizmo() {
}

bool Keela::TraceGizmo::on_button_press_event(GdkEventButton *button_event) {
    spdlog::debug(__func__);
    return DrawingArea::on_button_press_event(button_event);
}

bool Keela::TraceGizmo::on_button_release_event(GdkEventButton *release_event) {
    spdlog::debug(__func__);
    return DrawingArea::on_button_release_event(release_event);
}

bool Keela::TraceGizmo::on_motion_notify_event(GdkEventMotion *motion_event) {
    return DrawingArea::on_motion_notify_event(motion_event);
}

bool Keela::TraceGizmo::on_draw(const Cairo::RefPtr<Cairo::Context> &cr) {
    cr->save();
    cr->set_source_rgba(255, 0, 0, 1.0);
    cr->set_line_width(2);
    cr->save();
    cr->translate(bounds.get_x() / 2.0, bounds.get_y() / 2.0);
    cr->scale(bounds.get_width(), bounds.get_height());
    cr->translate(-bounds.get_width() / 2.0, -bounds.get_height() / 2.0);
    cr->arc(bounds.get_width() / 2.0, bounds.get_height() / 2.0, 1, 0, 2 * M_PI);
    cr->restore();
    cr->stroke();

    cr->restore();
    return DrawingArea::on_draw(cr);
}
