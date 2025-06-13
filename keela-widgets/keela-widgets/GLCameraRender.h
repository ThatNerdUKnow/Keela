//
// Created by brand on 6/13/25.
//

#ifndef GLCAMERARENDER_H
#define GLCAMERARENDER_H
#include <gtkmm/glarea.h>

namespace Keela {
    class GLCameraRender final : public Gtk::GLArea{
    public:
        GLCameraRender();
        ~GLCameraRender() override;
    protected:
        void on_realize() override;

        bool on_render(const Glib::RefPtr<Gdk::GLContext> &context) override;
    private:
        float vertices[9]={  -0.5f,-0.5f,0.0f,
                            0.5f,-0.5f,0.0f,
                            0.0f,0.5f,0.0f};
        unsigned int VBO;
    };
}
#endif //GLCAMERARENDER_H
