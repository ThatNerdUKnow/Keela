//
// Created by brand on 6/13/25.
//

#include "keela-widgets/GLCameraRender.h"
#include <glad/glad.h>
Keela::GLCameraRender::GLCameraRender() {


}

Keela::GLCameraRender::~GLCameraRender() {
}

void Keela::GLCameraRender::on_realize() {
    // stateful nonsense
    GLArea::on_realize();
    make_current();

    // these segfault for some reason
    //glGenBuffers(1,&VBO);
    //glBindBuffer(GL_ARRAY_BUFFER,VBO);
    //glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

}

bool Keela::GLCameraRender::on_render(const Glib::RefPtr<Gdk::GLContext> &context) {
    return GLArea::on_render(context);
}
