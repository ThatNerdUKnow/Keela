//
// Created by brand on 5/30/2025.
//

#ifndef CAMERACONTROLWINDOW_H
#define CAMERACONTROLWINDOW_H
#include <gtkmm-3.0/gtkmm.h>
#include <keela-widgets/labeledcomboboxtext.h>
#include <keela-widgets/labeledspinbutton.h>

#include "../keela-widgets/keela-widgets/cameramanager.h"
#include "keela-widgets/GLCameraRender.h"
#include "keela-widgets/GLTraceRender.h"
#include "keela-widgets/tracegizmo.h"

namespace Keela {
    class CameraControlWindow final : public Gtk::Window, public Keela::ITraceable {
    public:
        explicit CameraControlWindow(guint id);

        ~CameraControlWindow() override;

        std::shared_ptr<Keela::CameraManager> camera_manager;

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
        Gtk::CheckButton split_frames_check = Gtk::CheckButton("Split Even/Odd Frames");
        Gtk::Button fetch_image_button = Gtk::Button("Fetch Image");
        std::unique_ptr<Keela::GLCameraRender> gl_area;
        std::unique_ptr<Keela::GLCameraRender> gl_area2;

        std::shared_ptr<Keela::TraceGizmo> trace_gizmo;
        guint id;

        void on_range_check_toggled();

        void on_rotation_changed();

        void on_flip_horiz_changed() const;

        void on_flip_vert_changed() const;

        void on_split_frames_changed() const;

    public:
        std::shared_ptr<TraceBin> get_trace_bin() override;

        std::shared_ptr<TraceGizmo> get_trace_gizmo() override;

        std::string get_name() override;

    private:
        // set width and height initially to a sentinel value
        int m_width = -1, m_height = -1;
    };
}  // namespace Keela

#define ROTATION_NONE "No_Rotation"
#define ROTATION_90 "Rotation_90"
#define ROTATION_180 "Rotation_180"
#define ROTATION_270 "Rotation_270"

#endif  // CAMERACONTROLWINDOW_H
