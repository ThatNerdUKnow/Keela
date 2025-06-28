//
// Created by brand on 6/25/2025.
//

#include "keela-widgets/tracegizmo.h"

#include <spdlog/spdlog.h>


Keela::TraceGizmo::TraceGizmo() {
    add_events(Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::BUTTON_PRESS_MASK);
    //center = Gdk::Point(250, 250);
    bounds = Gdk::Rectangle(250, 250, 100, 200);

    auto off_x = HALF(bounds.get_width());
    auto off_y = HALF(bounds.get_height());

    auto center_x = bounds.get_x() + off_x;
    auto center_y = bounds.get_y() + off_y;
    ctrl_top_left = std::make_unique<GizmoControl>(center_x - off_x, center_y - off_y);
    ctrl_top_right = std::make_unique<GizmoControl>(center_x + off_x, center_y - off_y);
    ctrl_bottom_left = std::make_unique<GizmoControl>(center_x - off_x, center_y + off_y);
    ctrl_bottom_right = std::make_unique<GizmoControl>(center_x + off_x, center_y + off_y);

    ctrl_top_left->set_peer_y(*ctrl_top_right);
    ctrl_top_right->set_peer_y(*ctrl_top_left);
    ctrl_bottom_left->set_peer_y(*ctrl_bottom_right);
    ctrl_bottom_right->set_peer_y(*ctrl_bottom_left);

    ctrl_top_left->set_peer_x(*ctrl_bottom_left);
    ctrl_bottom_left->set_peer_x(*ctrl_top_left);
    ctrl_top_right->set_peer_x(*ctrl_bottom_right);
    ctrl_bottom_right->set_peer_x(*ctrl_top_right);
}

Keela::TraceGizmo::~TraceGizmo() {
}

bool Keela::TraceGizmo::on_button_press_event(GdkEventButton *button_event) {
    spdlog::debug(__func__);
    Gdk::Point point(button_event->x, button_event->y);
    if (ctrl_top_left->intersects(point)) {
        mode = top_left;
    } else if (ctrl_top_right->intersects(point)) {
        mode = top_right;
    } else if (ctrl_bottom_left->intersects(point)) {
        mode = bottom_left;
    } else if (ctrl_bottom_right->intersects(point)) {
        mode = bottom_right;
    } else {
        mode = none;
    }
    return DrawingArea::on_button_press_event(button_event);
}

bool Keela::TraceGizmo::on_button_release_event(GdkEventButton *release_event) {
    spdlog::debug(__func__);
    mode = none;
    return DrawingArea::on_button_release_event(release_event);
}

bool Keela::TraceGizmo::on_motion_notify_event(GdkEventMotion *motion_event) {
    Gdk::Point pt(motion_event->x, motion_event->y);

    if (ctrl_top_left != nullptr & ctrl_top_right != nullptr & ctrl_bottom_left != nullptr & ctrl_bottom_right !=
        nullptr) {
        ctrl_top_left->set_hovered((ctrl_top_left->intersects(pt) & mode == none) || mode == top_left);
        ctrl_top_right->set_hovered((ctrl_top_right->intersects(pt) & mode == none) || mode == top_right);
        ctrl_bottom_left->set_hovered((ctrl_bottom_left->intersects(pt) & mode == none) || mode == bottom_left);
        ctrl_bottom_right->set_hovered((ctrl_bottom_right->intersects(pt) & mode == none) || mode == bottom_right);

        GizmoControl *control = nullptr;
        switch (mode) {
            case top_left:
                control = ctrl_top_left.get();
                break;
            case top_right:
                control = ctrl_top_right.get();
                break;
            case bottom_left:
                control = ctrl_bottom_left.get();
                break;
            case bottom_right:
                control = ctrl_bottom_right.get();
                break;
            case none:
                return DrawingArea::on_motion_notify_event(motion_event);
        }
        assert(control != nullptr);
        control->set_center(pt);
    }

    bounds.set_x(ctrl_top_left->get_center().get_x());
    bounds.set_y(ctrl_top_left->get_center().get_y());

    auto width = ctrl_top_right->get_x() - ctrl_top_left->get_x();
    auto height = ctrl_bottom_left->get_y() - ctrl_top_left->get_y();
    bounds.set_width(width);
    bounds.set_height(height);
    return DrawingArea::on_motion_notify_event(motion_event);
}

bool Keela::TraceGizmo::on_draw(const Cairo::RefPtr<Cairo::Context> &cr) {
    cr->save();
    cr->set_source_rgb(1, 0, 0);
    cr->set_line_width(2);
    if (ctrl_top_left != nullptr & ctrl_top_right != nullptr & ctrl_bottom_left != nullptr & ctrl_bottom_right !=
        nullptr) {
        //cr->set_source_rgb(0, 1, 0);
        ctrl_top_left->draw(cr);
        //cr->set_source_rgb(1, 0, 0);
        ctrl_top_right->draw(cr);
        ctrl_bottom_left->draw(cr);
        ctrl_bottom_right->draw(cr);
    }
    cr->save();
    cr->translate(bounds.get_x() + HALF(bounds.get_width()), bounds.get_y() + HALF(bounds.get_height()));
    cr->scale(HALF(bounds.get_width()), HALF(bounds.get_height()));
    cr->translate(-HALF(bounds.get_x()), -HALF(bounds.get_y()));
    cr->arc(HALF(bounds.get_x()), HALF(bounds.get_y()), 1, 0, 2 * M_PI);
    cr->restore();
    cr->stroke();

    cr->set_source_rgb(0, 0, 1);
    //cr->rectangle(bounds.get_x(), bounds.get_y(), bounds.get_width(), bounds.get_height());
    //cr->stroke();
    cr->restore();
    return DrawingArea::on_draw(cr);
}
