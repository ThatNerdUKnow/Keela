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
        //GLfloat x;
        GLfloat y;
    };

    /**
     * Abstract base class: A source that can be rendered in the trace window
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

        void set_framerate(double framerate);

    private:
        Gtk::GLArea gl_area;
        Gtk::Label label;

        // *basically* a pointer to the camera control window
        std::shared_ptr<Keela::ITraceable> trace;

        unsigned int shader_program;

        void on_gl_realize();

        bool on_gl_render(const Glib::RefPtr<Gdk::GLContext> &context);

        unsigned int VAO;
        unsigned int VBO;

        std::string vertex_shader_source;
        std::string fragment_shader_source;

        std::jthread worker_thread;
        std::mutex worker_mutex;
        std::vector<PlotPoint> plot_points;

        /**
         * target length of plot_points buffer. Should equal PLOT_DURATION_SEC * framerate
         */
        unsigned long long plot_length;

        const int PLOT_DURATION_SEC = 10;
        /**
         * function to be used in worker_thread in order to process video data
         * @param token
         */
        void process_video_data(std::stop_token token);
    };
}
#endif //GLTRACERENDER_H
