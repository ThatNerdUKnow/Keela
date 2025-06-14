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

    // make sure opengl symbols are loaded before we do any opengl stuff
    if (!gladLoadGL()) {
        throw std::runtime_error("Failed to load OpenGL symbols");
    }

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW);

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vertex_cstr = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vertex_cstr, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::stringstream ss;
        ss << "Vertex shader compilation failed: " << infoLog;
        throw std::runtime_error(ss.str());
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragment_cstr = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &fragment_cstr, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::stringstream ss;
        ss << "Fragment shader compilation failed: " << infoLog;
        throw std::runtime_error(ss.str());
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram,GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::stringstream ss;
        ss << "Program linking failed: " << infoLog;
        throw std::runtime_error(ss.str());
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // linking vertex attributes
    // ReSharper disable once CppZeroConstantCanBeReplacedWithNullptr
    glVertexAttribPointer(0, 3,GL_FLOAT,GL_FALSE, 3 * sizeof(float), static_cast<void *>(0));
    glEnableVertexAttribArray(0);
}

bool Keela::GLCameraRender::on_render(const Glib::RefPtr<Gdk::GLContext> &context) {
    glUseProgram(shaderProgram);

    return GLArea::on_render(context);
}
