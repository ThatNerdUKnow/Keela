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
#include "keela-pipeline/consts.h"

namespace Keela {
    /**
     * Abstract base class: A source that can be rendered in the trace window
     */
    class ITraceable {
    public:
        virtual ~ITraceable() = default;

        virtual std::shared_ptr<TraceBin> get_trace_bin() = 0;

        virtual std::shared_ptr<TraceGizmo> get_trace_gizmo() = 0;

        virtual std::string get_name() = 0;
    };

    class GLTraceRender final : public Gtk::Box {
    public:
        explicit GLTraceRender(const std::shared_ptr<ITraceable> &cam_to_trace);

        ~GLTraceRender() override;

        void set_trace_render_framerate(double framerate);

    private:

        Gtk::GLArea gl_area;
        Gtk::Label name_label;
        Gtk::Label min_label;
        Gtk::Label max_label;

        // *basically* a pointer to the camera control window
        std::shared_ptr<Keela::ITraceable> trace;

        unsigned int shader_program{};

        void on_gl_realize();

        bool on_gl_render(const Glib::RefPtr<Gdk::GLContext> &context);

        unsigned int VAO = -1;
        unsigned int VBO = -1;

        std::string vertex_shader_source;
        std::string fragment_shader_source;

        std::jthread worker_thread;
        std::mutex worker_mutex;
        std::deque<float> plot_points;

        const int PLOT_DURATION_SEC = 10;
        /**
         * target length of plot_points buffer. Should equal PLOT_DURATION_SEC * framerate
         */
        unsigned long long plot_length = DEFAULT_TRACE_FPS * PLOT_DURATION_SEC;
        float plot_max = 255;
        float plot_min = 0;


        /**
         * function to be used in worker_thread in order to process video data
         * @param token
         */
        void process_video_data(const std::stop_token &token);

        template<typename T>
        double calculate_roi_average(GstSample *sample, GstStructure *structure, std::endian endianness);
    };
}
#endif //GLTRACERENDER_H
