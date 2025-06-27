//
// Created by brand on 6/26/2025.
//

#ifndef GIZMOCONTROL_H
#define GIZMOCONTROL_H
#include <gtkmm-3.0/gtkmm.h>

namespace Keela {
    class GizmoControl : public Gdk::Rectangle {
    public:
        GizmoControl(int x, int y);

        ~GizmoControl();

        void draw(const Cairo::RefPtr<Cairo::Context> &cr) const;

        bool intersects(const Gdk::Point &position) const;

        void set_hovered(bool hovered);

    private:
        bool hovered;
    };
}
#endif //GIZMOCONTROL_H
