//
// Created by brand on 5/26/2025.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm-3.0/gtkmm.h>

class MainWindow final : public Gtk::Window {

    public:
    MainWindow();
    ~MainWindow() override;
    private:
    Gtk::Button record_button;
    Gtk::Button restart_camera_button;

    Gtk::Box framerate_box;
    Gtk::Label framerate_label;
    Gtk::SpinButton framerate_spin;

    Gtk::Frame data_matrix_frame;
    Gtk::Grid data_matrix_grid;
    Gtk::Label data_matrix_w_label;
    Gtk::Label data_matrix_h_label;
    Gtk::SpinButton data_matrix_w_spin;
    Gtk::SpinButton data_matrix_h_spin;


    Gtk::CheckButton cv_recording_check;
    Gtk::Button fetch_image_button;
    Gtk::CheckButton lock_camera_check;
    Gtk::SpinButton num_camera_spin;
    Gtk::CheckButton show_trace_check;

    Gtk::Box container;

};



#endif //MAINWINDOW_H
