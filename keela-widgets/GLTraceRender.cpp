//
// Created by brand on 6/30/2025.
//

#include "keela-widgets/GLTraceRender.h"

#include <spdlog/spdlog.h>

#include "glad/glad.h"

Keela::GLTraceRender::GLTraceRender(const std::shared_ptr<ITraceable> &cam_to_trace): Gtk::Box(
    Gtk::ORIENTATION_VERTICAL) {
    spdlog::info(__func__);
    label.set_text(cam_to_trace->get_name());
    Container::add(label);
    Container::add(gl_area);
    gl_area.set_size_request(300, 128);
    trace = cam_to_trace;

    spdlog::debug("{}: Loading vertex shader resource", __func__);
    GError *error = NULL;
    auto vertex_res = g_resources_lookup_data("/org/gatech/keela/shaders/trace-vertex.glsl",
                                              G_RESOURCE_LOOKUP_FLAGS_NONE, &error);
    if (!vertex_res && !error) {
        std::stringstream ss;
        ss << "Could not load vertex shader resource: " << error->message;
        throw std::runtime_error(ss.str());
    }
    auto vertex_shader_size = g_bytes_get_size(vertex_res);
    auto vertex_shader_data = g_bytes_get_data(vertex_res, &vertex_shader_size);
    auto vertex_shader_dup = g_strndup(static_cast<const gchar *>(vertex_shader_data), vertex_shader_size);
    vertex_shader_source = std::string(vertex_shader_dup);
    g_bytes_unref(vertex_res);
    g_free(vertex_shader_dup);

    spdlog::debug("{}: Loading fragment shader resource", __func__);
    auto fragment_res = g_resources_lookup_data("/org/gatech/keela/shaders/trace-fragment.glsl",
                                                G_RESOURCE_LOOKUP_FLAGS_NONE, &error);
    if (!fragment_res && !error) {
        std::stringstream ss;
        ss << "Could not load fragment shader resource: " << error->message;
        throw std::runtime_error(ss.str());
    }

    auto fragment_shader_size = g_bytes_get_size(fragment_res);
    auto fragment_shader_data = g_bytes_get_data(fragment_res, &fragment_shader_size);
    auto fragment_shader_dup = g_strndup(static_cast<const gchar *>(fragment_shader_data), fragment_shader_size);
    fragment_shader_source = std::string(fragment_shader_dup);
    g_bytes_unref(fragment_res);
    g_free(fragment_shader_dup);

    gl_area.signal_realize().connect(sigc::mem_fun(this, &GLTraceRender::on_realize));
    show_all();
}

Keela::GLTraceRender::~GLTraceRender() = default;

void Keela::GLTraceRender::on_realize() {
    spdlog::info("GLTraceRender::{}", __func__);
    if (!Box::get_realized()) {
        spdlog::warn("GLTraceRender::{}: Not realized", __func__);
        Box::on_realize();
        return;
    }
    gl_area.make_current();
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // TODO: generate static data for now

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vertex_cstr = vertex_shader_source.c_str();
    glShaderSource(vertexShader, 1, &vertex_cstr, nullptr);
    glCompileShader(vertexShader);

    int succes;
    char infolog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS, &succes);
    if (!succes) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infolog);
        std::stringstream ss;
        ss << "Vertex shader compilation failed:\n " << infolog;
        spdlog::error(ss.str());
        throw std::runtime_error(ss.str());
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragment_cstr = fragment_shader_source.c_str();
    glShaderSource(fragmentShader, 1, &fragment_cstr, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS, &succes);
    if (!succes) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infolog);
        std::stringstream ss;
        ss << "Fragment shader compilation failed:\n " << infolog;
        spdlog::error(ss.str());
        throw std::runtime_error(ss.str());
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram,GL_LINK_STATUS, &succes);
    if (!succes) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infolog);
        std::stringstream ss;
        ss << "Program linking failed:\n " << infolog;
        spdlog::error(ss.str());
        throw std::runtime_error(ss.str());
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // setup attribute pointers
    spdlog::info("GLTraceRender::{} successfully realized", __func__);
}
