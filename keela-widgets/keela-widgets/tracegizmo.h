//
// Created by brand on 6/25/2025.
//

#ifndef TRACEGIZMO_H
#define TRACEGIZMO_H
#include <gtkmm/drawingarea.h>

#include "gizmocontrol.h"

namespace Keela {
    class TraceGizmo : public Gtk::DrawingArea {
    public:
        TraceGizmo();

        ~TraceGizmo() override;

        bool intersects(int x, int y) const;

        bool get_enabled() const;

    protected:
        bool on_button_press_event(GdkEventButton *button_event) override;

        bool on_button_release_event(GdkEventButton *release_event) override;

        bool on_motion_notify_event(GdkEventMotion *motion_event) override;

        bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) override;

    private:
        std::unique_ptr<Gdk::Rectangle> bounds = nullptr;
        bool is_dragging = false;
        bool is_enabled = true;

        enum edit_mode {
            top_left,
            top_right,
            bottom_left,
            bottom_right,
            none
        };

        edit_mode mode = none;

        std::unique_ptr<Keela::GizmoControl> ctrl_top_left = nullptr;
        std::unique_ptr<Keela::GizmoControl> ctrl_top_right = nullptr;
        std::unique_ptr<Keela::GizmoControl> ctrl_bottom_left = nullptr;
        std::unique_ptr<Keela::GizmoControl> ctrl_bottom_right = nullptr;
    };
}
#endif //TRACEGIZMO_H
