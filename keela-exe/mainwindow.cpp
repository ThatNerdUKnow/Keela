//
// Created by brand on 5/26/2025.
//

#include "mainwindow.h"
#include <keela-widgets/labeledspinbutton.h>
#include <spdlog/spdlog.h>
#include "cameracontrolwindow.h"
#include "keela-widgets/framebox.h"

MainWindow::MainWindow(): Gtk::Window() {
    set_title("Main Control Window");
    set_resizable(true);
    set_default_size(800, 600);
    container.set_orientation(Gtk::ORIENTATION_VERTICAL);
    container.set_spacing(10);
    container.set_homogeneous(false);
    container.set_border_width(10);
    MainWindow::add(container);

    // Record button
    record_button.set_label("Start Recording");
    record_button.signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_record_button_clicked));
    Gdk::RGBA red;
    red.set_red(1.0);
    red.set_alpha(1.0);
    record_button.override_color(red);
    container.add(record_button);

    // Framerate controls
    framerate_spin.m_spin.set_adjustment(Gtk::Adjustment::create(500, 1, 1000, 0.1));
    framerate_spin.m_spin.set_digits(1);
    container.add(framerate_spin);

    const auto dm_frame = Gtk::make_managed<Keela::FrameBox>("Data Matrix Dimensions", Gtk::ORIENTATION_VERTICAL);
    dm_frame->set_spacing(10);
    data_matrix_w_spin.m_spin.set_adjustment(Gtk::Adjustment::create(720.0, 1.0, std::numeric_limits<double>::max()));
    data_matrix_h_spin.m_spin.set_adjustment(Gtk::Adjustment::create(540.0, 1.0, std::numeric_limits<double>::max()));
    dm_frame->add(data_matrix_w_spin);
    dm_frame->add(data_matrix_h_spin);
    container.add(*dm_frame);

    // calcium voltage recording setting control
    cv_recording_check.set_label("Calcium-Voltage Recording Setting");
    container.add(cv_recording_check);

    const auto camera_limits = Gtk::Adjustment::create(1.0, 1.0, std::numeric_limits<double>::max());
    num_camera_spin.m_spin.set_adjustment(camera_limits);
    num_camera_spin.m_spin.signal_value_changed().connect(sigc::mem_fun(*this, &MainWindow::on_camera_spin_changed));
    container.add(num_camera_spin);

    show_trace_check.set_label("Show Traces");
    container.add(show_trace_check);

    restart_camera_button.signal_clicked().connect(sigc::mem_fun(this, &MainWindow::reset_cameras));
    restart_camera_button.set_label("Restart Camera(s)");
    container.add(restart_camera_button);

    // create the pipeline
    pipeline = GST_PIPELINE(gst_pipeline_new("pipeline"));
    if (!pipeline) {
        throw std::runtime_error("Failed to create pipeline");
    }

    // Initialize recording settings here
    on_camera_spin_changed();
    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
    show_all_children();
}

MainWindow::~MainWindow() {
    g_object_unref(pipeline);
}

void MainWindow::on_camera_spin_changed() {
    const auto curr = cameras.size();
    const auto next = num_camera_spin.m_spin.get_value_as_int();
    auto ret = gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_NULL);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        spdlog::error("Failed to set state of pipeline");
    }
    if (next > curr) {
        for (guint i = curr; i < next; i++) {
            auto camera_id = i + 1;
            auto c = std::make_unique<Keela::CameraControlWindow>(camera_id);
            set_framerate(c->camera_manager.get());
            set_resolution(c.get());
            g_object_ref(static_cast<GstElement*>(*c->camera_manager));
            auto ret = gst_bin_add(GST_BIN(pipeline), *c->camera_manager);
            if (!ret) {
                std::stringstream ss = std::stringstream();
                ss << "Failed to add camera " << std::to_string(camera_id) << " to pipeline";
                throw std::runtime_error(ss.str());
            }
            cameras.push_back(std::move(c));
        }
    } else if (next < curr) {
        for (guint i = curr; i > next; i--) {
            auto camera_win = std::move(cameras.back());
            gst_bin_remove(GST_BIN(pipeline), *camera_win->camera_manager);
            cameras.pop_back();
        }
    }
    ret = gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        spdlog::error("Failed to set state of pipeline");
    }
    gst_debug_bin_to_dot_file(GST_BIN(pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "keelapipeline");
}

void MainWindow::on_record_button_clicked() {
    is_recording = !is_recording;
    auto button_text = is_recording ? "Recording" : "Start Recording";
    record_button.set_label(button_text);
    // set_sensitive
    framerate_spin.set_sensitive(!is_recording);
    data_matrix_w_spin.set_sensitive(!is_recording);
    data_matrix_h_spin.set_sensitive(!is_recording);
    cv_recording_check.set_sensitive(!is_recording);
    num_camera_spin.set_sensitive(!is_recording);
    show_trace_check.set_sensitive(!is_recording);
    restart_camera_button.set_sensitive(!is_recording);
    if (is_recording) {
        auto message_dialog = Gtk::MessageDialog("Remember to set the experiment output to a new directory");
        message_dialog.run();
        // TODO: show file dialog
    } else {
        auto message_dialog = Gtk::MessageDialog("Remember to take calibration photos");
        message_dialog.run();
    }
}

void MainWindow::reset_cameras() {
    auto ret = gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_NULL);
    switch (ret) {
        case GST_STATE_CHANGE_FAILURE:
            throw std::runtime_error("Failed to set state of pipeline to NULL");
        case GST_STATE_CHANGE_ASYNC:
            ret = gst_element_get_state(GST_ELEMENT(pipeline), nullptr, nullptr, GST_CLOCK_TIME_NONE);
            if (ret == GST_STATE_CHANGE_FAILURE) {
                throw std::runtime_error("Async state change failed");
            }
            break;
    }
    // TODO: apply changes to camera settings here
    set_framerate();
    set_resolution();

    ret = gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        throw std::runtime_error("Failed to set state of pipeline to playing");
    }
}

void MainWindow::set_framerate() const {
    for (const auto &c: cameras) {
        set_framerate(c->camera_manager.get());
    }
}

void MainWindow::set_framerate(Keela::CameraManager *cm) const {
    const auto fr = framerate_spin.m_spin.get_value();
    cm->set_framerate(fr);
}

void MainWindow::set_resolution() const {
    const auto w = data_matrix_w_spin.m_spin.get_value_as_int();
    const auto h = data_matrix_h_spin.m_spin.get_value_as_int();
    for (const auto &c: cameras) {
        set_resolution(c.get());
        //set_resolution(c->camera_manager.get());
        //c->camera_manager->set_resolution(w, h);
    }
}

void MainWindow::set_resolution(Keela::CameraControlWindow *c) const {
    const auto w = data_matrix_w_spin.m_spin.get_value_as_int();
    const auto h = data_matrix_h_spin.m_spin.get_value_as_int();
    c->set_resolution(w, h);
}
