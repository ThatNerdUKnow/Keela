//
// Created by brand on 5/26/2025.
//

#include "mainwindow.h"
#include <keela-widgets/labeledspinbutton.h>
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
    container.add(framerate_spin);

    const auto dm_frame = Gtk::make_managed<Gtk::Frame>("Data Matrix Dimensions");
    const auto dm_box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
    dm_box->set_spacing(10);
    dm_frame->add(*dm_box);
    dm_box->add(data_matrix_w_spin);
    dm_box->add(data_matrix_h_spin);
    container.add(*dm_frame);

    // calcium voltage recording setting control
    cv_recording_check.set_label("Calcium-Voltage Recording Setting");
    container.add(cv_recording_check);

    fetch_image_button.set_label("Fetch Image from Camera");
    container.add(fetch_image_button);

    lock_camera_check.set_label("Lock Camera Number");
    container.add(lock_camera_check);

    const auto camera_limits = Gtk::Adjustment::create(1.0, 1.0, std::numeric_limits<double>::max());
    num_camera_spin.m_spin.set_adjustment(camera_limits);
    container.add(num_camera_spin);

    show_trace_check.set_label("Show Traces");
    container.add(show_trace_check);

    restart_camera_button.set_label("Restart Camera(s)");
    container.add(restart_camera_button);
    show_all_children();
}

MainWindow::~MainWindow()
{
}
