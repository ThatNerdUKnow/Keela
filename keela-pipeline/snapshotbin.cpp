//
// Created by brand on 6/10/2025.
//

#include "keela-pipeline/snapshotbin.h"

#include <sstream>

#include "keela-pipeline/gst-helpers.h"
#include "keela-pipeline/utils.h"

Keela::SnapshotBin::SnapshotBin() {
    SnapshotBin::init();
    SnapshotBin::link();
}

Keela::SnapshotBin::~SnapshotBin() {
    spdlog::debug(__func__);
}

void Keela::SnapshotBin::take_snapshot() {
    spdlog::info(__func__);
    GstSample *sample;
    g_signal_emit_by_name(app_sink, "pull-sample", &sample);

    if (!sample) {
        throw std::runtime_error("Failed to get sample from appsink");
    }
    GstFlowReturn ret;
    g_signal_emit_by_name(app_source, "push-sample", sample, &ret);
    gst_sample_unref(sample);
}

GstPadProbeReturn Keela::SnapshotBin::appsink_caps_changed(GstPad *pad, GstPadProbeInfo *info, SnapshotBin *usr) {
    assert(usr != nullptr);
    GstEvent *event = GST_PAD_PROBE_INFO_EVENT(info);
    if (GST_EVENT_TYPE(event) == GST_EVENT_CAPS) {
        GstCaps *caps;
        gst_event_parse_caps(event, &caps);
        char *caps_str = gst_caps_to_string(caps);
        spdlog::info("{} caps changed on appsrc\n{}", __func__, caps_str);
        g_free(caps_str);
        spdlog::debug("{} copying caps to app_source", __func__);
        g_object_set(usr->app_source, "caps", caps, nullptr);
        auto parent = gst_element_get_parent(usr);
        if (parent) {
            auto name = GST_ELEMENT_NAME(parent);
            std::stringstream ss;
            ss << name << ".dot";
            spdlog::debug("{} restarting parent {}", __func__, name);
            gst_element_set_state(GST_ELEMENT(parent), GST_STATE_PLAYING);
            gst_debug_bin_to_dot_file(GST_BIN(parent), GST_DEBUG_GRAPH_SHOW_ALL, ss.str().c_str());
            g_object_unref(parent);
        }
    }
    // TODO: dump bin dot graph to disk
    return GST_PAD_PROBE_OK;
}

void Keela::SnapshotBin::init() {
    g_object_set(app_sink, "max-buffers", 1, nullptr);
    g_object_set(app_sink, "drop", true, nullptr);
    // set up callback to automatically copy caps of appsink to appsrc
    GstPad *sinkpad = gst_element_get_static_pad(app_sink, "sink");
    gst_pad_add_probe(sinkpad, GST_PAD_PROBE_TYPE_EVENT_DOWNSTREAM,
                      reinterpret_cast<GstPadProbeCallback>(appsink_caps_changed), this,
                      nullptr);
}

void Keela::SnapshotBin::link() {
    add_elements(app_sink, app_source, file_sink);
    link_queue(app_sink);
    element_link_many(app_source, file_sink);
    //add_ghost_pad(app_sink, "sink");
}
