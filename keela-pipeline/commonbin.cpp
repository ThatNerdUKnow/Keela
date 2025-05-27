//
// Created by brand on 5/26/2025.
//

#include "keela-pipeline/commonbin.h"

Keela::CommonBin::CommonBin() {
}

Keela::CommonBin::CommonBin(std::string name, bool subsample) {
}

Keela::CommonBin::~CommonBin() {
    g_object_unref(scale);
    g_object_unref(tee);
}
