//
// Created by brand on 6/26/2025.
//

#ifndef GIZMOCONTROL_H
#define GIZMOCONTROL_H
#include <gtkmm-3.0/gtkmm.h>

#define HALF(x) x / 2.0

namespace Keela {
    class GizmoControl : public Gdk::Rectangle {
    public:
        GizmoControl();

        GizmoControl(int x, int y);

        ~GizmoControl();

        void draw(const Cairo::RefPtr<Cairo::Context> &cr) const;

        bool intersects(const Gdk::Point &position) const;

        void set_hovered(bool hovered);

        void set_center(Gdk::Point center);

        Gdk::Point get_center() const;

        /**
        * bind two GizmoControls together to follow each other on the x axis
        * @param peer
        */
        void set_peer_x(GizmoControl &peer);

        /**
         * bind two GizmoControls together to follow each other on the y axis
         * @param peer
         */
        void set_peer_y(GizmoControl &peer);

    private:
        bool hovered;
        static constexpr int width = 15;
        GizmoControl *peer_x = nullptr;
        GizmoControl *peer_y = nullptr;
    };
}
#endif //GIZMOCONTROL_H
