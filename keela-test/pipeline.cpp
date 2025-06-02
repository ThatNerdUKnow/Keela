//
// Created by brand on 5/27/2025.
//
#include <gtest/gtest.h>
#include <keela-pipeline/recordbin.h>
#include <keela-pipeline/presentationbin.h>
#include <keela-pipeline/transformbin.h>
#include <spdlog/spdlog.h>

#include "keela-pipeline/testsrc.h"
TEST(KeelaPipeline, ConstructBin) {
    auto bin = Keela::Bin();
}

TEST(KeelaPipeline, ConstructNamedBin) {
    auto bin = Keela::Bin("Foo");
}


TEST(KeelaPipeline, ConstructRecordBin) {
    auto bin = Keela::RecordBin();
}

TEST(KeelaPipeline, ConstructNamedRecordBin) {
    auto bin = Keela::RecordBin("Foo");
}

TEST(KeelaPipeline, ConstructPresentationBin) {
    auto bin = Keela::PresentationBin();
}

TEST(KeelaPipeline, ConstructNamedPresentationBin) {
    auto bin = Keela::PresentationBin("Foo");
}

TEST(KeelaPipeline, ConstructTransformBin) {
    auto bin = Keela::TransformBin();
}

TEST(KeelaPipeline, ConstructNamedTransformBin) {
    auto bin = Keela::TransformBin("Foo");
}

#pragma region demonstrably false tests
TEST(KeelaPipeline, UseBinAsGstBin) {
    GstBin* bin = Keela::Bin();
    if (!GST_IS_BIN(bin))
        GTEST_SKIP();
}

TEST(KeelaPipeline, UseBinAsGstElement) {
    GstElement* bin = Keela::Bin();
    if (!GST_IS_ELEMENT(bin))
        GTEST_SKIP();
}
#pragma endregion

TEST(KeelaPipeline, DuplicateNamedBins) {
    auto bin1 = Keela::Bin("Foo");
    auto bin2 = Keela::Bin("Foo");
}

TEST(KeelaPipeline, LinkBins) {
    auto bin1 = Keela::TransformBin();
    auto bin2 = Keela::RecordBin();
    // TODO: Set ghost pads on Keela::Bin
    ASSERT_TRUE(gst_element_link(bin1,bin2));
}

TEST(KeelaPipeline, ConstructTestSrc) {
    auto src = Keela::TestSrc();
}

TEST(KeelaPipeline, ConstructNamedTestSrc) {
    auto src = Keela::TestSrc("Foo");
}

TEST(KeelaPipeline, CanPlay) {
    Keela::Bin bin = Keela::Bin();
    Keela::TestSrc src = Keela::TestSrc();
    Keela::TransformBin transform = Keela::TransformBin();
    Keela::PresentationBin presentation = Keela::PresentationBin();
    spdlog::info("Created all elements");
    // TODO: link elements and set pipeline to playing
    // FIXME: my IDE is being really funky and marking any gstreamer calls after this point get marked as unreachable
}