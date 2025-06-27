//
// Created by brand on 6/25/2025.
//

#ifndef TRACEGIZMO_H
#define TRACEGIZMO_H
#include <gtkmm/drawingarea.h>

namespace Keela {
    class TraceGizmo : public Gtk::DrawingArea {
    public:
        TraceGizmo();

        ~TraceGizmo() override;

    protected:
        bool on_button_press_event(GdkEventButton *button_event) override;

        bool on_button_release_event(GdkEventButton *release_event) override;

        bool on_motion_notify_event(GdkEventMotion *motion_event) override;

        bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) override;

    private:
        Gdk::Rectangle bounds;
        bool is_dragging = false;
        bool is_enabled = true;
    };
}
#endif //TRACEGIZMO_H
