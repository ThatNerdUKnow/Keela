//
// Created by brand on 6/13/25.
//
#include <gtest/gtest.h>

#include <keela-widgets/GLCameraRender.h>

TEST(KeelaWidgets, ConstructGLCameraRender) {
    auto bin = std::make_shared<Keela::PresentationBin>();
    auto c = Keela::GLCameraRender(bin);
}
