#include "keela-pipeline/CameraStreamBin.h"

#include <keela-pipeline/gtkglsink.h>
#include <keela-pipeline/gtksink.h>
#include <spdlog/spdlog.h>

#include <stdexcept>

#include "keela-pipeline/utils.h"

Keela::CameraStreamBin::CameraStreamBin(const std::string &name) : QueueBin(name), name(name) {
    spdlog::info("Creating CameraStreamBin: {}", name);
    presentation = std::make_shared<PresentationBin>("presentation_" + this->name);
    snapshot = std::make_shared<SnapshotBin>("snapshot_" + this->name);
    trace = std::make_shared<TraceBin>("trace_" + this->name);

    CameraStreamBin::init();
    CameraStreamBin::link();
}

Keela::CameraStreamBin::~CameraStreamBin() {
    spdlog::debug(__func__);
}

void Keela::CameraStreamBin::init() {
    // Just configure elements, don't link yet
    spdlog::info("Initializing CameraStreamBin {} elements", name);
}

void Keela::CameraStreamBin::link() {
    spdlog::info("Linking CameraStreamBin {} internal structure", name);

    // Add internal tee and all child bins to this bin
    add_elements(internal_tee, *presentation, *snapshot, *trace);

    // Link internal_tee -> all outputs
    element_link_many(internal_tee, *presentation);
    element_link_many(internal_tee, *snapshot);
    element_link_many(internal_tee, *trace);
    link_queue(internal_tee);

    spdlog::info("CameraStreamBin linked: queue -> tee -> presentation");
}
