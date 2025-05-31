//
// Created by brand on 5/26/2025.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm-3.0/gtkmm.h>

#include <keela-widgets/labeledspinbutton.h>

#include "cameracontrolwindow.h"

class MainWindow final : public Gtk::Window {

    public:
    MainWindow();
    ~MainWindow() override;
    private:
    Gtk::Button record_button;
    Gtk::Button restart_camera_button;

    Keela::LabeledSpinButton framerate_spin = Keela::LabeledSpinButton("Framerate (Hz)");
    Keela::LabeledSpinButton data_matrix_w_spin = Keela::LabeledSpinButton("Data Width");
    Keela::LabeledSpinButton data_matrix_h_spin = Keela::LabeledSpinButton("Data Height");



    Gtk::CheckButton cv_recording_check;
    Gtk::Button fetch_image_button;
    Gtk::CheckButton lock_camera_check;
    Keela::LabeledSpinButton num_camera_spin = Keela::LabeledSpinButton("Number of Cameras");
    Gtk::CheckButton show_trace_check;

    Gtk::Box container;

    std::vector<std::shared_ptr<CameraControlWindow>> cameras;
};



#endif //MAINWINDOW_H
