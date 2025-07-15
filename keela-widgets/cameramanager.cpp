//
// Created by brand on 6/3/2025.
//

#include "keela-widgets/cameramanager.h"

#include <sstream>
#include <stdexcept>
#include <spdlog/spdlog.h>

#include <keela-pipeline/utils.h>

#include "keela-pipeline/recordbin.h"

Keela::CameraManager::CameraManager(guint id, bool split_streams): Bin("camera_" + std::to_string(id)),
                                                                   camera("videotestsrc") {
    try {
        spdlog::info("Creating camera manager {}", id);
        this->id = id;

        gst_caps_set_simple(base_caps, "format",G_TYPE_STRING, "GRAY8", nullptr);
        g_object_set(caps_filter, "caps", static_cast<GstCaps *>(base_caps), nullptr);
        add_elements(camera, auto_video_convert, caps_filter, transform, tee, trace, *presentation, snapshot);
        element_link_many(camera, auto_video_convert, caps_filter, transform, tee, *presentation);
        element_link_many(tee, snapshot);
        element_link_many(tee, trace);

        spdlog::info("Created camera manager {}", id);
    } catch (const std::exception &e) {
        std::stringstream ss;

        ss << "camera" << id << ".dot";
        spdlog::error("could not create camera manager: {}\n attempting to dump bin to {}", e.what(), ss.str());
        dump_bin_graph();
        throw;
    }
}

Keela::CameraManager::~CameraManager() {
    spdlog::debug(__func__);
}

void Keela::CameraManager::set_framerate(double framerate) {
    spdlog::info("Setting framerate to {}", framerate);
    int numerator = static_cast<int>(framerate * 10);
    // TODO: caps need to be writable
    base_caps = Caps(static_cast<GstCaps *>(base_caps));
    base_caps.set_framerate(numerator, 10);
    //gst_caps_set_simple(base_caps, "framerate", GST_TYPE_FRACTION, numerator, 10, nullptr);

    // through experimentation, I believe that changes to the original caps reference do not affect the capsfilter
    g_object_set(caps_filter, "caps", static_cast<GstCaps *>(base_caps), nullptr);
}

void Keela::CameraManager::set_resolution(const int width, const int height) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("width and height must be greater than zero");
    }
    // create a copy of our current caps
    base_caps = Caps(static_cast<GstCaps *>(base_caps));
    base_caps.set_resolution(width, height);
    g_object_set(caps_filter, "caps", static_cast<GstCaps *>(base_caps), nullptr);
    transform.scale(width, height);
}

void Keela::CameraManager::set_experiment_directory(const std::string &path) {
    experiment_directory = path;
}

void Keela::CameraManager::start_recording() {
    std::shared_ptr<RecordBin> record_bin = std::make_shared<RecordBin>("recordbin");
    std::stringstream ss;
    ss << this->experiment_directory << "\\cam_" << std::to_string(this->id) << ".mkv";
    record_bin->set_directory(ss.str());
    add_elements(static_cast<GstElement *>(*record_bin));
    assert(gst_element_sync_state_with_parent(*record_bin));
    element_link_many(tee, static_cast<GstElement *>(*record_bin));
    record_bins.insert(record_bin);
}

void Keela::CameraManager::stop_recording() {
    // notes from example to dynamically remove a bin from a playing pipeline:
    // add a blocking downstream probe to the queue "src" pad
    // inside the blocking callback, add the EOS probe to the last source pad of the bin (it is unclear if this can also be a sink pad)
    // after installing the EOS callback, send an EOS event to the sink pad of the beginning of the bin
    // inside the EOS callback, set the state of the bin to NULL and remove the bin from the pipeline
    for (auto bin: record_bins) {
        auto copy = new std::shared_ptr(bin);
        // TODO: can this be the sink pad instead?
        const auto pad = gst_element_get_static_pad(bin->queue, "sink");
        assert(pad != nullptr);

        auto peer = gst_pad_get_peer(pad);
        assert(peer != nullptr);
        //auto peer_parent = gst_pad_get_parent_element(peer);
        spdlog::debug("peer is {}",GST_ELEMENT_NAME(peer));
        gst_pad_add_probe(peer,
                          GST_PAD_PROBE_TYPE_BLOCK_DOWNSTREAM,
                          pad_block_callback,
                          copy,
                          nullptr);
        g_object_unref(pad);
        g_object_unref(peer);
    }

    for (auto bin: record_bins) {
        auto lock = std::unique_lock(bin->remove_mutex);
        bin->remove_condition.wait(lock, [bin]() {
            return bin->safe_to_remove;
        });
        lock.unlock();
        auto state_ret = gst_element_set_state(*bin, GST_STATE_NULL);
        auto remove_ret = gst_bin_remove(*this, *bin);
        auto name = GST_ELEMENT_NAME(static_cast<GstElement*>(*this));
        if (state_ret == GST_STATE_CHANGE_FAILURE || !remove_ret) {
            spdlog::error("{} could not remove recordbin from pipeline", name);
        } else {
            spdlog::info("{} successfully removed recordbin from pipeline", name);
        }
    }
    record_bins.clear();
    spdlog::info("Removed all recordbins from pipeline");
    dump_bin_graph();
}

GstPadProbeReturn Keela::CameraManager::pad_block_callback(GstPad *pad, GstPadProbeInfo *info, gpointer user_data) {
    auto recordbin = static_cast<std::shared_ptr<RecordBin> *>(user_data);
    auto name = GST_ELEMENT_NAME(static_cast<GstElement*>(**recordbin));
    spdlog::info("Pad block received from {}", name);
    gst_pad_remove_probe(pad, GST_PAD_PROBE_INFO_ID(info));

    spdlog::debug("setting eos callback");
    auto file_sink_pad = gst_element_get_static_pad((*recordbin)->mux, "src");
    assert(file_sink_pad != nullptr);


    gst_pad_add_probe(file_sink_pad,
                      static_cast<GstPadProbeType>(GST_PAD_PROBE_TYPE_EVENT_DOWNSTREAM),
                      event_callback,
                      recordbin, nullptr);
    g_object_unref(file_sink_pad);

    spdlog::debug("sending EOS");

    auto queuepad = gst_element_get_static_pad((*recordbin)->queue, "sink");
    auto queue_peer = gst_pad_get_peer(queuepad);
    gst_pad_unlink(queue_peer, queuepad);
    gst_pad_send_event(queuepad, gst_event_new_eos());
    g_object_unref(queuepad);
    g_object_unref(queue_peer);
    (*recordbin)->dump_bin_graph();
    return GST_PAD_PROBE_OK;
}

GstPadProbeReturn Keela::CameraManager::event_callback(GstPad *pad, GstPadProbeInfo *info, gpointer user_data) {
    auto recordbin = static_cast<std::shared_ptr<RecordBin> *>(user_data);
    if (GST_EVENT_TYPE(GST_PAD_PROBE_INFO_DATA(info)) != GST_EVENT_EOS) {
        return GST_PAD_PROBE_OK;
    }
    auto name = GST_ELEMENT_NAME(static_cast<GstElement*>(**recordbin));
    spdlog::info("EOS received from {}", name);

    gst_pad_remove_probe(pad, GST_PAD_PROBE_INFO_ID(info));

    (*recordbin)->dump_bin_graph();
    // acquire lock and set signal variable
    {
        auto lock = std::scoped_lock((*recordbin)->remove_mutex);
        (*recordbin)->safe_to_remove = true;
    }
    (*recordbin)->remove_condition.notify_all();

    //gst_element_set_state(**recordbin, GST_STATE_NULL);
    // auto parent = GST_ELEMENT_PARENT(static_cast<GstElement*>(**recordbin));
    // because recordbin is supposed to be inside a pipeline, it should have a parent
    //assert(parent != nullptr);
    //gst_bin_remove(GST_BIN(parent), **recordbin);
    delete recordbin;
    return GST_PAD_PROBE_OK;
}
