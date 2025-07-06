//
// Created by brand on 6/30/2025.
//

#ifndef GLTRACERENDER_H
#define GLTRACERENDER_H
#include <gtkmm/box.h>
#include <gtkmm/glarea.h>
#include <gtkmm/label.h>

#include "cameramanager.h"
#include "tracegizmo.h"
#include "glad/glad.h"

namespace Keela {
    struct PlotPoint {
        GLfloat x;
        GLfloat y;
    };

    /**
     * A source that can be rendered in the trace window
     */
    class ITraceable {
    public:
        virtual ~ITraceable() = default;

        virtual std::shared_ptr<CameraManager> get_camera_manager() = 0;

        virtual std::shared_ptr<TraceGizmo> get_trace_gizmo() = 0;

        virtual std::string get_name() = 0;
    };

    class GLTraceRender final : public Gtk::Box {
    public:
        explicit GLTraceRender(const std::shared_ptr<ITraceable> &cam_to_trace);

        ~GLTraceRender() override;

    private:
        Gtk::GLArea gl_area;
        Gtk::Label label;
        std::shared_ptr<Keela::ITraceable> trace;
        unsigned int shader_program;

        void on_gl_realize();

        bool on_gl_render(const Glib::RefPtr<Gdk::GLContext> &context);

        unsigned int VAO;
        unsigned int VBO;

        std::string vertex_shader_source;
        std::string fragment_shader_source;
    };
}
#endif //GLTRACERENDER_H
