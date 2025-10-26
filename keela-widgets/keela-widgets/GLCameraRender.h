//
// Created by brand on 6/13/25.
//

#ifndef GLCAMERARENDER_H
#define GLCAMERARENDER_H
#include <gtkmm/glarea.h>
#include <gstreamer-1.0/gst/gst.h>
#include <keela-pipeline/presentationbin.h>

namespace Keela {
    /**
     * An object that can control the heatmap behavior of a GLCameraRender object
     */
    class IControlGLCameraRenderHeatmap {
    public:
        virtual ~IControlGLCameraRenderHeatmap() = default;

        virtual bool is_heatmap_enabled() = 0;

        virtual float heatmap_min() = 0;

        virtual float heatmap_max() = 0;
    };

    class GLCameraRender final : public Gtk::GLArea {
    public:
        explicit GLCameraRender(std::shared_ptr<PresentationBin> bin, IControlGLCameraRenderHeatmap &controller);

        ~GLCameraRender() override;

        void new_tex_sample(GstSample *sample);

    protected:
        void on_realize() override;

        bool on_render(const Glib::RefPtr<Gdk::GLContext> &context) override;

    private:
        std::string vertexShaderSource;
        std::string fragmentShaderSource;

        bool on_timeout();

        unsigned int VBO;
        unsigned int vertexShader;
        unsigned int fragmentShader;
        unsigned int shaderProgram;
        unsigned int VAO;
        unsigned int texture;
        std::shared_ptr<PresentationBin> bin;
        float vertices[24] = {
            -1, -1, 0, 1,
            1, 1, 1, 0,
            -1, 1, 0, 0,
            1, -1, 1, 1,
            1, 1, 1, 0,
            -1, -1, 0, 1
        };

        IControlGLCameraRenderHeatmap &m_controller;
    };
}
#endif //GLCAMERARENDER_H
