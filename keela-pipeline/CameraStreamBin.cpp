#include "keela-pipeline/CameraStreamBin.h"

#include <keela-pipeline/gtkglsink.h>
#include <keela-pipeline/gtksink.h>
#include <spdlog/spdlog.h>

#include <stdexcept>

#include "keela-pipeline/utils.h"

Keela::CameraStreamBin::CameraStreamBin(const std::string &name) : QueueBin(name), name(name) {
    spdlog::info("Creating CameraStreamBin: {}", name);
    presentation = std::make_shared<PresentationBin>("presentation_" + this->name);
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

    // Add internal tee and presentation to this bin (will add record, trace later)
    add_elements(internal_tee, *presentation);

    // Link internal_tee -> presentation  
    element_link_many(internal_tee, *presentation);
    link_queue(internal_tee);

    spdlog::info("CameraStreamBin linked: queue -> tee -> presentation");
}
