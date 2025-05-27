//
// Created by brand on 5/27/2025.
//

#include <gstreamer-1.0/gst/gst.h>
#include <gtest/gtest.h>
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    gst_init(&argc, &argv);
    return RUN_ALL_TESTS();
}