//
// Created by brand on 5/30/2025.
//

#include "cameracontrolwindow.h"

#include <spdlog/spdlog.h>

#include <keela-widgets/framebox.h>

Keela::CameraControlWindow::CameraControlWindow(const guint id) {
    this->id = id;
    spdlog::info("Creating {} for camera {}", __func__, id);

    camera_manager = std::make_unique<Keela::CameraManager>(id, false);
    set_title("Image control for Camera " + std::to_string(id));
    set_resizable(false);
    set_deletable(false);
    v_container.set_spacing(10);
    v_container.set_border_width(10);
    h_container.pack_start(v_container, false, false, 10);
    Window::add(h_container);
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
    v_container.add(*range_frame);

    gain_spin.m_spin.set_adjustment(Gtk::Adjustment::create(0.0, 0.0, 100));
    v_container.add(gain_spin);

    // TODO: add rotation options
    rotation_combo.m_combo.signal_changed().connect(sigc::mem_fun(*this, &CameraControlWindow::on_rotation_changed));
    rotation_combo.m_combo.append(ROTATION_NONE, "---");
    rotation_combo.m_combo.append(ROTATION_90, "Rotate 90 degrees clockwise");
    rotation_combo.m_combo.append(ROTATION_180, "Rotate 180 degrees");
    rotation_combo.m_combo.append(ROTATION_270, "Rotate 270 degrees clockwise");
    rotation_combo.m_combo.set_active_id(ROTATION_NONE);

    v_container.add(rotation_combo);
    flip_horiz_check.signal_toggled().connect(sigc::mem_fun(*this, &CameraControlWindow::on_flip_horiz_changed));
    v_container.add(flip_horiz_check);
    flip_vert_check.signal_toggled().connect(sigc::mem_fun(*this, &CameraControlWindow::on_flip_vert_changed));
    v_container.add(flip_vert_check);

    // TODO: dynamically cast camera_manager->presentation to a WidgetElement to get a handle to a widget to add to the window
    fetch_image_button.signal_clicked().connect(
        sigc::mem_fun(camera_manager->snapshot, &Keela::SnapshotBin::take_snapshot));
    v_container.add(fetch_image_button);

    gl_area = std::make_unique<GLCameraRender>(camera_manager->presentation);
    gl_area->set_size_request(640, 480);
    set_vexpand(false);
    auto overlay = Gtk::make_managed<Gtk::Overlay>();
    trace_gizmo = std::make_shared<TraceGizmo>();
    overlay->add(*gl_area);
    overlay->add_overlay(*trace_gizmo);
    h_container.pack_start(*overlay, false, false, 10);

    auto gl_bin = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
    h_container.pack_start(*gl_bin, false, false, 10);
    show_all_children();
    show();
}

Keela::CameraControlWindow::~CameraControlWindow() {
    spdlog::info("Destroying camera control window {}", id);
}

void Keela::CameraControlWindow::on_range_check_toggled() {
    const auto active = range_check.get_active();
    range_min_spin.set_sensitive(active);
    range_max_spin.set_sensitive(active);
}

void Keela::CameraControlWindow::set_resolution(const int width, const int height) {
    spdlog::trace("resolution changed");
    this->camera_manager->set_resolution(width, height);
    // TODO: can we set a minimum size or allow the user to scale the gl area themselves?
    const auto rotation = rotation_combo.m_combo.get_active_id();
    if (rotation == ROTATION_90 || rotation == ROTATION_270) {
        gl_area->set_size_request(height, width);
    } else {
        gl_area->set_size_request(width, height);
    }
    m_width = width;
    m_height = height;
    // force the window to recalculate its size with respect to the size requests of its children
    resize(1, 1);
}

void Keela::CameraControlWindow::on_rotation_changed() {
    spdlog::trace("rotation changed");
    // NOTE: this appears to mess with the caps of the video stream
    const auto value = rotation_combo.m_combo.get_active_id();
    if (value == ROTATION_NONE) {
        camera_manager->transform.rotate_identity();
    } else if (value == ROTATION_90) {
        camera_manager->transform.rotate_90();
    } else if (value == ROTATION_180) {
        camera_manager->transform.rotate_180();
    } else if (value == ROTATION_270) {
        camera_manager->transform.rotate_270();
    } else {
        throw std::runtime_error(value + "is an invalid rotation");
    }
    if (m_width > 0 && m_height > 0) {
        set_resolution(m_width, m_height);
    } else {
        spdlog::warn("Skipping resolution change - resolution not initialized");
    }
}

void Keela::CameraControlWindow::on_flip_horiz_changed() const {
    const auto value = flip_horiz_check.get_active();
    camera_manager->transform.flip_horizontal(value);
}

void Keela::CameraControlWindow::on_flip_vert_changed() const {
    const auto value = flip_vert_check.get_active();
    camera_manager->transform.flip_vertical(value);
}

std::shared_ptr<Keela::TraceBin> Keela::CameraControlWindow::get_trace_bin() {
    return camera_manager->trace;
}

std::shared_ptr<Keela::TraceGizmo> Keela::CameraControlWindow::get_trace_gizmo() {
    return trace_gizmo;
}

std::string Keela::CameraControlWindow::get_name() {
    std::stringstream ss;
    ss << "Camera " << std::to_string(id);
    return ss.str();
}
