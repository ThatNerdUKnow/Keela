//
// Created by brand on 6/26/2025.
//

#include "keela-widgets/gizmocontrol.h"

Keela::GizmoControl::GizmoControl(const int x, const int y): Gdk::Rectangle(x, y, 15, 15) {
}

Keela::GizmoControl::~GizmoControl() {
}

void Keela::GizmoControl::draw(const Cairo::RefPtr<Cairo::Context> &cr) const {
    cr->save();

    cr->rectangle(get_x(), get_y(), get_width(), get_height());
    if (hovered) {
        cr->fill();
    } else {
        cr->stroke();
    }
    cr->restore();
}

bool Keela::GizmoControl::intersects(const Gdk::Point &position) const {
    auto rect = Gdk::Rectangle(position.get_x(), position.get_y(), 1, 1);
    return Gdk::Rectangle::intersects(rect);
}

void Keela::GizmoControl::set_hovered(const bool hovered) {
    this->hovered = hovered;
}
