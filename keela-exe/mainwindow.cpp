//
// Created by brand on 5/26/2025.
//

#include "mainwindow.h"
#include <keela-widgets/labeledspinbutton.h>
#include <spdlog/spdlog.h>

#include "keela-widgets/framebox.h"

MainWindow::MainWindow(): Gtk::Window()
{
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

    const auto dm_frame = Gtk::make_managed<Keela::FrameBox>("Data Matrix Dimensions",Gtk::ORIENTATION_VERTICAL);
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
    num_camera_spin.m_spin.signal_value_changed().connect(sigc::mem_fun(*this,on_camera_spin_changed));
    container.add(num_camera_spin);

    show_trace_check.set_label("Show Traces");
    container.add(show_trace_check);

    restart_camera_button.set_label("Restart Camera(s)");
    container.add(restart_camera_button);

    //auto w = std::make_unique<CameraControlWindow>(1);
    //cameras.push_back(std::move(w));
    on_camera_spin_changed();
    show_all_children();
}

MainWindow::~MainWindow()
= default;

void MainWindow::on_camera_spin_changed() {
    const auto curr = cameras.size();
    const auto next = num_camera_spin.m_spin.get_value_as_int();
    if (next > curr) {
        for (guint i = curr; i < next; i++) {
            auto c = std::make_unique<CameraControlWindow>(i + 1);
            cameras.push_back(std::move(c));
        }
    } else if (next < curr) {
        for (guint i = curr; i > next; i--) {
            cameras.pop_back();
        }
    }
}
