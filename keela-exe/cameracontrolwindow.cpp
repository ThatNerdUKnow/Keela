//
// Created by brand on 5/30/2025.
//

#include "cameracontrolwindow.h"

CameraControlWindow::CameraControlWindow(const int id) {
    set_title("Image control for Camera " + std::to_string(id));
    Window::add(container);
    range_check = Gtk::CheckButton("Range");
    container.add(range_check);

    container.add(range_min_spin);
    container.add(range_max_spin);


    container.add(gain_spin);

    // TODO: add rotation options
    container.add(rotation_combo);
    container.add(flip_horiz_check);
    container.add(flip_vert_check);
    show_all_children();
}
