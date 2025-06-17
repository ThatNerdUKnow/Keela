//
// Created by brand on 6/13/25.
//

#include "keela-widgets/GLCameraRender.h"
#include <glad/glad.h>
#include <gio/gio.h>
#include <spdlog/spdlog.h>

Keela::GLCameraRender::GLCameraRender() {
    GError *error = nullptr;
    spdlog::debug("Loading vertex shader resource");
    auto vertex_res = g_resources_lookup_data("/org/gatech/keela/shaders/hello-triangle-vertex.glsl",
                                              G_RESOURCE_LOOKUP_FLAGS_NONE, &error);
    if (!vertex_res && !error) {
        std::stringstream ss;
        ss << "Could not load vertex shader resource: " << error->message;
        throw std::runtime_error(ss.str());
    }
    spdlog::debug("Loading fragment shader resource");
    auto fragment_res = g_resources_lookup_data("/org/gatech/keela/shaders/hello-triangle-fragment.glsl",
                                                G_RESOURCE_LOOKUP_FLAGS_NONE, &error);
    if (!fragment_res && !error) {
        std::stringstream ss;
        ss << "Could not load fragment shader resource: " << error->message;
        throw std::runtime_error(ss.str());
    }

    auto vertex_shader_size = g_bytes_get_size(vertex_res);
    auto vertex_shader_data = g_bytes_get_data(vertex_res, &vertex_shader_size);
    auto vertex_shader_dup = g_strndup(static_cast<const gchar *>(vertex_shader_data), vertex_shader_size);

    vertexShaderSource = std::string(vertex_shader_dup);
    // uncommenting these appear to crash the program. it's somewhat unclear who owns these
    g_bytes_unref(vertex_res);
    g_free(vertex_shader_dup);

    auto frag_shader_size = g_bytes_get_size(fragment_res);
    auto frag_shader_data = g_bytes_get_data(fragment_res, &frag_shader_size);
    auto frag_shader_dup = g_strndup(static_cast<const gchar *>(frag_shader_data), frag_shader_size);

    fragmentShaderSource = std::string(frag_shader_dup);
    g_bytes_unref(fragment_res);
    g_free(frag_shader_dup);
}

Keela::GLCameraRender::~GLCameraRender() {
    spdlog::debug(__func__);
}

void Keela::GLCameraRender::on_realize() {
    // stateful nonsense
    GLArea::on_realize();
    make_current();

    // make sure opengl symbols are loaded before we do any opengl stuff
    if (!gladLoadGL()) {
        throw std::runtime_error("Failed to load OpenGL symbols");
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

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
        spdlog::error("error in vertex shader\n{}", vertexShaderSource);
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
        spdlog::error("error in fragment shader\n{}", fragmentShaderSource);
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

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

bool Keela::GLCameraRender::on_render(const Glib::RefPtr<Gdk::GLContext> &context) {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    float greenValue = (1) / 2.0f + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    glUseProgram(shaderProgram);
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    return GLArea::on_render(context);
}
