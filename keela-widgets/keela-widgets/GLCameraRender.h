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
        float vertices[9] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
        };

        // TODO: look into gio/resource to embed the shader code into our binary
        const std::string vertexShaderSource = "#version 330 core\n"
                "layout (location = 0) in vec3 aPos;\n"
                "void main()\n"
                "{\n"
                "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                "}\0";

        const std::string fragmentShaderSource = "#version 330 core\n"
                "out vec4 FragColor;\n"
                "void main()\n"
                "{\n"
                "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                "}\0";

        unsigned int VBO;
        unsigned int vertexShader;
        unsigned int fragmentShader;
        unsigned int shaderProgram;
    };
}
#endif //GLCAMERARENDER_H
