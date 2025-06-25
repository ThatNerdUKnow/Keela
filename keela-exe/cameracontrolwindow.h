//
// Created by brand on 5/30/2025.
//

#ifndef CAMERACONTROLWINDOW_H
#define CAMERACONTROLWINDOW_H
#include <gtkmm-3.0/gtkmm.h>
#include <keela-widgets/labeledspinbutton.h>
#include <keela-widgets/labeledcomboboxtext.h>

#include "cameramanager.h"
#include "keela-widgets/GLCameraRender.h"
#include "keela-widgets/tracegizmo.h"

namespace Keela {
    class CameraControlWindow final : public Gtk::Window {
    public:
        explicit CameraControlWindow(guint id);

        ~CameraControlWindow() override;

        std::unique_ptr<Keela::CameraManager> camera_manager;

        void set_resolution(int width, int height);

    private:
        Gtk::Box h_container = Gtk::Box();
        Gtk::Box v_container = Gtk::Box(Gtk::ORIENTATION_VERTICAL);
        Gtk::CheckButton range_check = Gtk::CheckButton("Range");
        Keela::LabeledSpinButton range_min_spin = Keela::LabeledSpinButton("Minimum");
        Keela::LabeledSpinButton range_max_spin = Keela::LabeledSpinButton("Maximum");

        // TODO: histogram
        Keela::LabeledSpinButton gain_spin = Keela::LabeledSpinButton("Gain");

        Keela::LabeledComboBoxText rotation_combo = Keela::LabeledComboBoxText("Select Rotation");
        Gtk::CheckButton flip_horiz_check = Gtk::CheckButton("Flip Along Horizontal Center");
        Gtk::CheckButton flip_vert_check = Gtk::CheckButton("Flip Along Vertical Center");
        Gtk::Button fetch_image_button = Gtk::Button("Fetch Image");
        std::unique_ptr<Keela::GLCameraRender> gl_area;
        Keela::TraceGizmo gizmo;
        guint id;


        void on_range_check_toggled();


        void on_rotation_changed();

        void on_flip_horiz_changed() const;

        void on_flip_vert_changed() const;

        // set width and height initially to a sentinel value
        int m_width = -1, m_height = -1;
    };
}

#define ROTATION_NONE "No_Rotation"
#define ROTATION_90 "Rotation_90"
#define ROTATION_180 "Rotation_180"
#define ROTATION_270 "Rotation_270"


#endif //CAMERACONTROLWINDOW_H
