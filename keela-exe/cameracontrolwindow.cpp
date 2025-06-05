//
// Created by brand on 5/30/2025.
//

#include "cameracontrolwindow.h"

#include <spdlog/spdlog.h>

#include <keela-pipeline/widgetelement.h>
#include <keela-widgets/framebox.h>

CameraControlWindow::CameraControlWindow(const guint id) {
    this->id = id;
    spdlog::info("Creating {} for camera {}", __func__, id);
    set_title("Image control for Camera " + std::to_string(id));
    set_default_size(640, 480);
    set_deletable(false);
    container.set_spacing(10);
    container.set_border_width(10);
    Window::add(container);
    const auto range_frame = Gtk::make_managed<Keela::FrameBox>("Range", Gtk::ORIENTATION_VERTICAL);
    range_check = Gtk::CheckButton("Range");
    // TODO: connect to changed signal on range check to control sensitivity value of range min/max spin
    range_check.signal_toggled().connect(sigc::mem_fun(*this, &CameraControlWindow::on_range_check_toggled));
    range_frame->add(range_check);

    range_min_spin.set_sensitive(false);
    range_max_spin.set_sensitive(false);
    range_min_spin.m_spin.set_adjustment(Gtk::Adjustment::create(0.0, 0.0, std::numeric_limits<uint8_t>::max()));
    range_max_spin.m_spin.set_adjustment(
        Gtk::Adjustment::create(std::numeric_limits<uint8_t>::max(), 0.0, std::numeric_limits<uint8_t>::max()));
    range_frame->add(range_min_spin);
    range_frame->add(range_max_spin);
    container.add(*range_frame);

    gain_spin.m_spin.set_adjustment(Gtk::Adjustment::create(0.0, 0.0, 100));
    container.add(gain_spin);

    // TODO: add rotation options
    rotation_combo.m_combo.append(ROTATION_NONE, "---");
    rotation_combo.m_combo.append(ROTATION_90, "Rotate 90 degrees clockwise");
    rotation_combo.m_combo.append(ROTATION_180, "Rotate 180 degrees");
    rotation_combo.m_combo.append(ROTATION_270, "Rotate 270 degrees clockwise");
    rotation_combo.m_combo.set_active_id(ROTATION_NONE);

    container.add(rotation_combo);
    container.add(flip_horiz_check);
    container.add(flip_vert_check);
    camera_manager = std::make_unique<Keela::CameraManager>(id, false);

    // TODO: dynamically cast camera_manager->presentation to a WidgetElement to get a handle to a widget to add to the window
    container.add(fetch_image_button);
    show_all_children();
    show();
}

CameraControlWindow::~CameraControlWindow() {
    spdlog::info("Destroying camera control window {}", id);
}

void CameraControlWindow::on_range_check_toggled() {
    const auto active = range_check.get_active();
    range_min_spin.set_sensitive(active);
    range_max_spin.set_sensitive(active);
}
