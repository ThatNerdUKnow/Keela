//
// Created by brand on 6/13/25.
//

#include "keela-widgets/GLCameraRender.h"

Keela::GLCameraRender::GLCameraRender() {

    //glGenBuffers(1,&VBO);
}

Keela::GLCameraRender::~GLCameraRender() {
}

void Keela::GLCameraRender::on_realize() {
    GLArea::on_realize();
}

bool Keela::GLCameraRender::on_render(const Glib::RefPtr<Gdk::GLContext> &context) {
    return GLArea::on_render(context);
}
