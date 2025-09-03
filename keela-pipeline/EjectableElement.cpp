//
// Created by brand on 9/1/2025.
//

#include "keela-pipeline/EjectableElement.h"

void Keela::EjectableElement::PrepareEject() {
    // TODO: can this be the sink pad instead?
    auto head = Head();
    const auto pad = gst_element_get_static_pad(head, "sink");
    assert(pad != nullptr);

    auto peer = gst_pad_get_peer(pad);
    assert(peer != nullptr);
    //auto peer_parent = gst_pad_get_parent_element(peer);
    spdlog::debug("peer is {}",GST_ELEMENT_NAME(peer));
    gst_pad_add_probe(peer,
                      GST_PAD_PROBE_TYPE_BLOCK_DOWNSTREAM,
                      pad_block_callback,
                      this,
                      nullptr);
    g_object_unref(pad);
    g_object_unref(peer);
}

void Keela::EjectableElement::Eject() {
    auto lock = std::unique_lock(remove_mutex);
    remove_condition.wait(lock, [this]() {
        return this->safe_to_remove;
    });
    lock.unlock();

    auto name = GST_ELEMENT_NAME(static_cast<GstElement*>(*this));
    auto state_ret = gst_element_set_state(GST_ELEMENT(static_cast<GstElement*>(*this)), GST_STATE_NULL);
    auto parent = GST_ELEMENT_PARENT(static_cast<GstElement*>(*this));
    if (GST_IS_BIN(parent)) {
        throw std::runtime_error("Could not eject element from pipeline. Element has no parent");
    }
    auto remove_ret = gst_bin_remove(GST_BIN(parent), *this);
    if (state_ret == GST_STATE_CHANGE_FAILURE || !remove_ret) {
        spdlog::error("could not remove {} from pipeline", name);
    } else {
        spdlog::info("successfully removed {} from pipeline", name);
    }
}

GstPadProbeReturn Keela::EjectableElement::pad_block_callback(GstPad *pad, GstPadProbeInfo *info, gpointer user_data) {
    auto bin = static_cast<Keela::EjectableElement *>(user_data);
    auto name = GST_ELEMENT_NAME(static_cast<GstElement*>(*bin));
    spdlog::info("Pad block received from {}", name);
    gst_pad_remove_probe(pad, GST_PAD_PROBE_INFO_ID(info));

    spdlog::debug("setting eos callback");
    auto tail = bin->Tail();
    auto tailpad = gst_element_get_static_pad(tail, "src");
    assert(tailpad != nullptr);


    gst_pad_add_probe(tailpad,
                      static_cast<GstPadProbeType>(GST_PAD_PROBE_TYPE_EVENT_DOWNSTREAM),
                      event_callback,
                      bin, nullptr);
    g_object_unref(tailpad);

    spdlog::debug("sending EOS");

    auto head = bin->Head();
    auto headpad = gst_element_get_static_pad(head, "sink");
    auto headpad_peer = gst_pad_get_peer(headpad);
    gst_pad_unlink(headpad_peer, headpad);
    gst_pad_send_event(headpad, gst_event_new_eos());
    g_object_unref(headpad);
    g_object_unref(headpad_peer);
    return GST_PAD_PROBE_OK;
}

GstPadProbeReturn Keela::EjectableElement::event_callback(GstPad *pad, GstPadProbeInfo *info, gpointer user_data) {
    auto bin = static_cast<EjectableElement *>(user_data);
    if (GST_EVENT_TYPE(GST_PAD_PROBE_INFO_DATA(info)) != GST_EVENT_EOS) {
        return GST_PAD_PROBE_OK;
    }
    auto name = GST_ELEMENT_NAME(static_cast<GstElement*>(*bin));
    spdlog::info("EOS received from {}", name);

    gst_pad_remove_probe(pad, GST_PAD_PROBE_INFO_ID(info));

    // acquire lock and set signal variable
    {
        auto lock = std::scoped_lock(bin->remove_mutex);
        bin->safe_to_remove = true;
    }
    bin->remove_condition.notify_all();

    return GST_PAD_PROBE_OK;
}
