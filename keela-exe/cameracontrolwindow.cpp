//
// Created by brand on 5/30/2025.
//

#include "cameracontrolwindow.h"

CameraControlWindow::CameraControlWindow(const int id) {
    set_title("Image control for Camera " + std::to_string(id));
    set_default_size(640, 480);
    set_deletable(false);
    Window::add(container);
    range_check = Gtk::CheckButton("Range");
    container.add(range_check);

    container.add(range_min_spin);
    container.add(range_max_spin);


    container.add(gain_spin);

    // TODO: add rotation options
    rotation_combo.m_combo.append(ROTATION_NONE,"---");
    rotation_combo.m_combo.append(ROTATION_90, "Rotate 90 degrees clockwise");
    rotation_combo.m_combo.append(ROTATION_180, "Rotate 180 degrees");
    rotation_combo.m_combo.append(ROTATION_270, "Rotate 270 degrees clockwise");
    rotation_combo.m_combo.set_active_id(ROTATION_NONE);

    container.add(rotation_combo);
    container.add(flip_horiz_check);
    container.add(flip_vert_check);
    container.add(fetch_image_button);
    show_all_children();
    show();
}
