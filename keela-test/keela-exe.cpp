#include <gtest/gtest.h>

#include "../keela-exe/cameramanager.h"
//
// Created by brand on 6/3/2025.
//
TEST(KeelaExe, ConstructCameraManager) {
    Keela::CameraManager cm(1, false);
    GstElement *b = cm;
    gst_debug_bin_to_dot_file(GST_BIN(b), GST_DEBUG_GRAPH_SHOW_ALL, "cameramanager1");
}
