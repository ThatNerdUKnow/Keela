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

    gl_area.signal_realize().connect(sigc::mem_fun(this, &GLTraceRender::on_gl_realize));
    gl_area.signal_render().connect(sigc::mem_fun(this, &GLTraceRender::on_gl_render));
    show_all();
}

Keela::GLTraceRender::~GLTraceRender() = default;

void Keela::GLTraceRender::on_gl_realize() {
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
    PlotPoint graph[2000];
    for (int i = 0; i < 2000; i++) {
        float x = i;
        graph[i].x = x;
        graph[i].y = sin(x);
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(graph), graph, GL_STATIC_DRAW);

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

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertexShader);
    glAttachShader(shader_program, fragmentShader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program,GL_LINK_STATUS, &succes);
    if (!succes) {
        glGetProgramInfoLog(shader_program, 512, nullptr, infolog);
        std::stringstream ss;
        ss << "Program linking failed:\n " << infolog;
        spdlog::error(ss.str());
        throw std::runtime_error(ss.str());
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // setup attribute pointers
    // location 0 is a vec2 representing a point in non-clip coordinates
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, // attribute
        2, // number of elements per vertex, here (x,y)
        GL_FLOAT, // the type of each element
        GL_FALSE, // take our values as-is
        0, // no space between values
        0 // use the vertex buffer object
    );
    // uint is a uniform representing the number of samples
    spdlog::info("GLTraceRender::{} successfully realized", __func__);
}

bool Keela::GLTraceRender::on_gl_render(const Glib::RefPtr<Gdk::GLContext> &context) {
    gl_area.make_current();
    glUseProgram(shader_program);
    glBindVertexArray(VAO);
    auto loc = glGetUniformLocation(shader_program, "numSamples");
    glUniform1ui(loc, 2000);
    glDrawArrays(GL_LINE_STRIP, 0, 2000);
    return true;
}
