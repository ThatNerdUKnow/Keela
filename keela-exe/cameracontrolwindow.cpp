//
// Created by brand on 5/30/2025.
//

#include "cameracontrolwindow.h"

#include <keela-widgets/framebox.h>
#include <spdlog/spdlog.h>

Keela::CameraControlWindow::CameraControlWindow(const guint id) {
    this->id = id;
    spdlog::info("Creating {} for camera {}", __func__, id);
    bool split_frames = false;
    camera_manager = std::make_unique<Keela::CameraManager>(id, split_frames);

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

    // Add frame splitting control
    split_frames_check.signal_toggled().connect(sigc::mem_fun(*this, &CameraControlWindow::on_split_frames_changed));
    split_frames_check.set_active(camera_manager->is_frame_splitting_enabled());
    v_container.add(split_frames_check);

    // TODO: dynamically cast camera_manager->presentation to a WidgetElement to get a handle to a widget to add to the window
    fetch_image_button.signal_clicked().connect(
        sigc::mem_fun(camera_manager->snapshot, &Keela::SnapshotBin::take_snapshot));
    v_container.add(fetch_image_button);

    set_vexpand(false);

    // Create overlay for trace gizmo
    trace_gizmo_even = std::make_shared<TraceGizmo>();

    // Set up video presentations, frame_widget_even renders all frames unless split is enabled
    frame_widget_even = std::make_unique<VideoPresentation>(
        "Camera " + std::to_string(id),
        camera_manager->presentation_even,
        640,    // width
        480     // height
    );
    frame_widget_even->add_overlay_widget(*trace_gizmo_even);
    video_hbox.pack_start(*frame_widget_even, false, false, 10);

    if (camera_manager->is_frame_splitting_enabled()) {
        add_split_frame_ui();
    }

    h_container.pack_start(video_hbox, false, false, 10);

    auto gl_bin = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
    h_container.pack_start(*gl_bin, false, false, 10);

    // Initialize traces
    update_traces();

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
        frame_widget_even->set_video_size(height, width);
        if (frame_widget_odd) {
            frame_widget_odd->set_video_size(height, width);
        }
    } else {
        frame_widget_even->set_video_size(width, height);
        if (frame_widget_odd) {
            frame_widget_odd->set_video_size(width, height);
        }
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

void Keela::CameraControlWindow::on_split_frames_changed() {
    const auto value = split_frames_check.get_active();
    camera_manager->set_frame_splitting(value);
    if (value) {
        spdlog::info("Adding split frame UI");
        add_split_frame_ui();
    } else {
        spdlog::info("Removing split frame UI");
        remove_split_frame_ui();
    }
    // Update traces whenever split frame state changes
    update_traces();
}

std::vector<std::shared_ptr<Keela::ITraceable>> Keela::CameraControlWindow::get_traces() {
    std::vector<std::shared_ptr<ITraceable>> traces;
    traces.reserve(m_traces.size());
    for (const auto& trace : m_traces) {
        traces.push_back(trace);
    }
    return traces;
}

void Keela::CameraControlWindow::update_traces() {
    m_traces.clear();
    
    // Always add the even trace
    std::string even_name = "Camera " + std::to_string(id);
    if (camera_manager->is_frame_splitting_enabled()) {
        even_name += " (Even)";
    }
    
    auto even_trace = std::make_shared<CameraTrace>(
        camera_manager->get_trace_even(),
        trace_gizmo_even,
        even_name
    );
    m_traces.push_back(even_trace);
    
    // Add odd trace if frame splitting is enabled
    if (camera_manager->is_frame_splitting_enabled() && trace_gizmo_odd) {
        auto odd_trace = std::make_shared<CameraTrace>(
            camera_manager->get_trace_odd(),
            trace_gizmo_odd,
            "Camera " + std::to_string(id) + " (Odd)"
        );
        m_traces.push_back(odd_trace);
    }
    
    // Notify parent that traces have been updated
    if (on_traces_updated_callback) {
        on_traces_updated_callback();
    }
}

void Keela::CameraControlWindow::apply_trace_framerate(guint fps) {
    spdlog::info("Applying trace framerate {} to all traces for camera {}", fps, id);
    for (const auto &trace : m_traces) {
        auto trace_bin = trace->get_trace_bin();
        if (trace_bin) {
            trace_bin->set_trace_framerate(fps);
        }
    }
}

void Keela::CameraControlWindow::add_split_frame_ui() {
    if (frame_widget_odd) return;  // Already added

    // Create trace gizmo for odd frames
    trace_gizmo_odd = std::make_shared<TraceGizmo>();

    const auto rotation = rotation_combo.m_combo.get_active_id();
    if (rotation == ROTATION_90 || rotation == ROTATION_270) {
        frame_widget_odd = std::make_unique<VideoPresentation>(
            "Odd Frames", 
            camera_manager->presentation_odd, 
            480,   // width
            640    // height
        );
    } else {
        frame_widget_odd = std::make_unique<VideoPresentation>(
            "Odd Frames", 
            camera_manager->presentation_odd,
            640,   // width
            480    // height
        );
    }

    frame_widget_odd->add_overlay_widget(*trace_gizmo_odd);
    video_hbox.pack_start(*frame_widget_odd, false, false, 10);

    // Ensure the new widget matches the current resolution
    if (m_width > 0 && m_height > 0) {
        const auto rotation = rotation_combo.m_combo.get_active_id();
        if (rotation == ROTATION_90 || rotation == ROTATION_270) {
            frame_widget_odd->set_video_size(m_height, m_width);
        } else {
            frame_widget_odd->set_video_size(m_width, m_height);
        }
    }

    show_all_children();
}

void Keela::CameraControlWindow::remove_split_frame_ui() {
    if (frame_widget_odd) {
        video_hbox.remove(*frame_widget_odd);
        frame_widget_odd.reset();
    }
}
