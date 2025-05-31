//
// Created by brand on 5/30/2025.
//

#ifndef CAMERACONTROLWINDOW_H
#define CAMERACONTROLWINDOW_H
#include <gtkmm-3.0/gtkmm.h>
#include <keela-widgets/labeledspinbutton.h>

class CameraControlWindow final: public Gtk::Window {
    public:
    explicit CameraControlWindow(int id);

private:
    Gtk::Box container = Gtk::Box(Gtk::ORIENTATION_VERTICAL);
    Gtk::CheckButton range_check = Gtk::CheckButton("Range");
    Keela::LabeledSpinButton range_min_spin = Keela::LabeledSpinButton("Minimum");
    Keela::LabeledSpinButton range_max_spin = Keela::LabeledSpinButton("Maximum");

    // TODO: histogram
    Keela::LabeledSpinButton gain_spin = Keela::LabeledSpinButton("Gain");
    // TODO: labeled combo box
    Gtk::ComboBox rotation_combo = Gtk::ComboBox();
    Gtk::CheckButton flip_horiz_check = Gtk::CheckButton("Flip Along Horizontal Center");
    Gtk::CheckButton flip_vert_check = Gtk::CheckButton("Flip Along Vertical Center");
};
#endif //CAMERACONTROLWINDOW_H
