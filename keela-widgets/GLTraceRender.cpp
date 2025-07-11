//
// Created by brand on 6/30/2025.
//

#include "keela-widgets/GLTraceRender.h"

#include <spdlog/spdlog.h>
#include <thread>
#include <utility>
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

    // start processor
    worker_thread = std::jthread([this](std::stop_token token) {
        this->process_video_data(std::move(token));
    });
}

Keela::GLTraceRender::~GLTraceRender() = default;

void Keela::GLTraceRender::set_framerate(double framerate) {
    spdlog::info("GLTraceRender::{}", __func__);
    // acquire mutex to avoid race conditions
    std::scoped_lock _(worker_mutex);
    // calculate new buffer size
    plot_length = static_cast<unsigned long long>(PLOT_DURATION_SEC * framerate);

    // determine if current buffer needs any modification
    if (plot_length < plot_points.size()) {
        // essentially discards the first *diff* elements from plot_points
        auto diff = plot_points.size() - plot_length;
        std::ranges::rotate(plot_points, plot_points.begin() + diff);
        plot_points.resize(plot_length);
    } else {
        // nothing to be done
    }
}

void Keela::GLTraceRender::on_gl_realize() {
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

    /*
    // TODO: generate static data for now
    for (int i = 0; i < 2000; i++) {
        //float x = i;
        float y = sin(i);
        PlotPoint p = {y};
        plot_points.push_back(p);
    }*/
    /*glBufferData(GL_ARRAY_BUFFER, static_cast<long long>(plot_points.size() * sizeof(PlotPoint)), plot_points.data(),
                 GL_STATIC_DRAW);*/

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
        0,
        1,
        GL_FLOAT,
        GL_FALSE,
        0,
        0
    );
    // uint is a uniform representing the number of samples
    spdlog::info("GLTraceRender::{} successfully realized", __func__);
}

bool Keela::GLTraceRender::on_gl_render(const Glib::RefPtr<Gdk::GLContext> &context) {
    std::scoped_lock _(worker_mutex);
    gl_area.make_current();
    glUseProgram(shader_program);
    glBindVertexArray(VAO);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    const auto loc = glGetUniformLocation(shader_program, "numSamples");
    glUniform1ui(loc, plot_length);
    glBufferData(GL_ARRAY_BUFFER, static_cast<long long>(plot_points.size() * sizeof(PlotPoint)), plot_points.data(),
                 GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINE_STRIP, 0, static_cast<int>(plot_points.size()));
    return true;
}

void Keela::GLTraceRender::process_video_data(std::stop_token token) {
    spdlog::debug("GLTraceRender::{}", __func__);
    GstElement *appsink = this->trace->get_camera_manager()->trace.sink;
    // might need to lock gizmo?
    guint64 current_num_buffers = 0;
    assert(appsink != nullptr);
    GstSample *sample = nullptr;

    while (!token.stop_requested()) {
        this->queue_draw();
        auto gizmo = this->trace->get_trace_gizmo();
        if (!gizmo->get_enabled()) {
            // gizmo is not currently active
            continue;
        }
        double mean = 0;
        g_signal_emit_by_name(appsink, "try-pull-sample", 0, &sample, nullptr);
        if (!sample) {
            // refresh data at most 30 times a second
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 30));
            continue;
        }

        auto buf = gst_sample_get_buffer(sample);
        assert(buf != nullptr);

        auto caps = gst_sample_get_caps(sample);
        assert(caps != nullptr);
        auto structure = gst_caps_get_structure(caps, 0);
        assert(structure != nullptr);
        gint width, height;
        assert(gst_structure_get_int(structure, "width", &width));
        assert(gst_structure_get_int(structure, "height", &height));


        GstMapInfo mapInfo;
        if (!gst_buffer_map(buf, &mapInfo, GST_MAP_READ)) {
            std::stringstream ss;
            ss << __func__ << "Buffer mapping failed";
            throw std::runtime_error(ss.str());
        }


        // do stuff with the buffer data
        double sum = 0;
        unsigned int count = 0;
        for (auto x = 0; x < width; x++) {
            for (auto y = 0; y < height; y++) {
                // multiply x and y by a factor of 2 since we are binning the camera pixels
                // NOTE: gizmo MAY be null
                if (gizmo->intersects(Gdk::Point(x * 2, y * 2))) {
                    count++;
                    auto i = y * width + x;
                    sum += mapInfo.data[i];
                }
            }
        }
        mean = sum / count;
        spdlog::debug("GLTraceRender::{}: {}", __func__, mean);

        gst_sample_unref(sample);
        sample = nullptr; {
            std::scoped_lock _(worker_mutex);
            if (plot_points.size() >= plot_length) {
                std::ranges::rotate(plot_points, plot_points.begin() + 1);
                plot_points.pop_back();
            }
            plot_points.push_back(PlotPoint(static_cast<float>(mean)));
        }
    }
    spdlog::info("GLTraceRender::{} stopping", __func__);
}
