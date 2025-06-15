//
// Created by brand on 6/13/25.
//

#ifndef GLCAMERARENDER_H
#define GLCAMERARENDER_H
#include <gtkmm/glarea.h>

namespace Keela {
    class GLCameraRender final : public Gtk::GLArea {
    public:
        GLCameraRender();

        ~GLCameraRender() override;

    protected:
        void on_realize() override;

        bool on_render(const Glib::RefPtr<Gdk::GLContext> &context) override;

    private:
        std::string vertexShaderSource;
        std::string fragmentShaderSource;

        unsigned int VBO;
        unsigned int vertexShader;
        unsigned int fragmentShader;
        unsigned int shaderProgram;
        unsigned int VAO;

        float vertices[18] = {
            // positions        // colors
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
        };
    };
}
#endif //GLCAMERARENDER_H
