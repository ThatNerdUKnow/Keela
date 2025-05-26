//
// Created by brand on 5/26/2025.
//

#include "mainwindow.h"

MainWindow::MainWindow(): Gtk::Window()
{
    set_title("Main Control Window");
    set_resizable(true);
    set_default_size(800, 600);
    container = Gtk::Box(Gtk::ORIENTATION_VERTICAL);
    MainWindow::add(container);

    // Record button
    record_button.set_label("Record");
    Gdk::RGBA red;
    red.set_red(1.0);
    red.set_alpha(1.0);
    record_button.override_color(red);
    container.add(record_button);

    // Framerate controls
    framerate_label = Gtk::Label("Framerate (hz)");
    framerate_spin = Gtk::SpinButton(Gtk::Adjustment::create(500,1.0,2000.0));
    framerate_spin.set_digits(0);

    framerate_box.add(framerate_spin);
    framerate_box.add(framerate_label);
    container.add(framerate_box);

    // Data matrix controls
    data_matrix_frame.set_label("Data Matrix Dimensions");
    data_matrix_frame.add(data_matrix_grid);
    data_matrix_w_label = Gtk::Label("Data Width");
    data_matrix_h_label = Gtk::Label("Data Height");
    auto dm_adj = Gtk::Adjustment::create(218.0,1.0,std::numeric_limits<double>::max());
    data_matrix_w_spin = Gtk::SpinButton(dm_adj);
    data_matrix_w_spin.set_digits(0);
    data_matrix_h_spin = Gtk::SpinButton(dm_adj);
    data_matrix_h_spin.set_digits(0);
    data_matrix_grid.attach(data_matrix_w_spin, 0, 0, 1, 1);
    data_matrix_grid.attach(data_matrix_w_label, 1, 0, 1, 1);

    data_matrix_grid.attach(data_matrix_h_spin, 0, 1, 1, 1);
    data_matrix_grid.attach(data_matrix_h_label, 1, 1, 1, 1);
    data_matrix_frame.add(data_matrix_w_spin);
    data_matrix_frame.add(data_matrix_h_spin);
    container.add(data_matrix_frame);


    show_all_children();

}

MainWindow::~MainWindow()
{
}
