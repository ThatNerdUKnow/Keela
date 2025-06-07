//
// Created by brand on 5/26/2025.
//

#include "mainwindow.h"
#include <keela-widgets/labeledspinbutton.h>
#include <spdlog/spdlog.h>

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
    record_button.set_label("Record");
    Gdk::RGBA red;
    red.set_red(1.0);
    red.set_alpha(1.0);
    record_button.override_color(red);
    container.add(record_button);

    // Framerate controls
    framerate_spin.m_spin.set_adjustment(Gtk::Adjustment::create(500, 1, 1000));
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

    restart_camera_button.set_label("Restart Camera(s)");
    container.add(restart_camera_button);

    // create the pipeline
    pipeline = GST_PIPELINE(gst_pipeline_new("pipeline"));
    if (!pipeline) {
        throw std::runtime_error("Failed to create pipeline");
    }
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
    auto ret = gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_READY);
    if (ret == GST_STATE_CHANGE_FAILURE)
    {
        throw std::runtime_error("Failed to set state of pipeline");
    }
    if (next > curr) {
        for (guint i = curr; i < next; i++) {
            auto camera_id = i+1;
            auto c = std::make_unique<CameraControlWindow>(camera_id);
            auto ret = gst_bin_add(GST_BIN(pipeline), *c->camera_manager);
            if (!ret)
            {
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
    if (ret == GST_STATE_CHANGE_FAILURE)
    {
        throw std::runtime_error("Failed to set state of pipeline");
    }
    gst_debug_bin_to_dot_file(GST_BIN(pipeline),GST_DEBUG_GRAPH_SHOW_ALL,"keelapipeline");
}
