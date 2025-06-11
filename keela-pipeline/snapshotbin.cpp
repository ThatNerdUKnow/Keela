//
// Created by brand on 6/10/2025.
//

#include "keela-pipeline/snapshotbin.h"

#include "keela-pipeline/utils.h"

Keela::SnapshotBin::SnapshotBin() {
    SnapshotBin::init();
    SnapshotBin::link();
}

Keela::SnapshotBin::~SnapshotBin() {
    spdlog::debug(__func__);
}

void Keela::SnapshotBin::take_snapshot(std::string filename) {
}

void Keela::SnapshotBin::init() {
    g_object_set(app_sink, "max-buffers", 1, nullptr);
    g_object_set(app_sink, "drop", true, nullptr);
}

void Keela::SnapshotBin::link() {
    add_elements(app_sink, app_source, file_sink);
    element_link_many(app_source, file_sink);
    add_ghost_pad(app_sink, "sink");
}
