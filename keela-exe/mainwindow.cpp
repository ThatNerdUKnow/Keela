//
// Created by brand on 5/26/2025.
//

#include "mainwindow.h"

MainWindow::MainWindow(): Gtk::Window()
{
    container = Gtk::Box();
    container.set_orientation(Gtk::Orientation::ORIENTATION_VERTICAL);
    MainWindow::add(container);

    record_button = Gtk::Button();
    record_button.set_label("Record");
    Gdk::RGBA red;
    red.set_red(1.0);
    red.set_alpha(1.0);
    record_button.override_background_color(red);
    container.add(record_button);

    framerate_spin = Gtk::SpinButton();
    framerate_spin.set_digits(0);
    framerate_spin.set_value(500);
    container.add(framerate_spin);
    show_all_children();

}

MainWindow::~MainWindow()
{
}
