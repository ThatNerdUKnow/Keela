//
// Created by brand on 5/27/2025.
//
#include <gtest/gtest.h>
#include <keela-pipeline/recordbin.h>
#include <keela-pipeline/presentationbin.h>
#include <keela-pipeline/transformbin.h>

#include "keela-pipeline/testsrc.h"

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
    Keela::Bin bin;
    ASSERT_NO_THROW(bin = Keela::Bin());
    Keela::TestSrc src;
    g_object_set(src,"num-buffers",100,nullptr);
    ASSERT_NO_THROW(src = Keela::TestSrc());
    Keela::TransformBin transform;
    ASSERT_NO_THROW(transform = Keela::TransformBin());
    Keela::PresentationBin presentation;
    ASSERT_NO_THROW(presentation = Keela::PresentationBin());
    gst_bin_add_many(bin,src,transform,presentation,nullptr);

    GstCaps *caps = gst_caps_new_simple ("video/x-raw",
     "format", G_TYPE_STRING, "GRAY8",
     "framerate", GST_TYPE_FRACTION, 500, 1,
     "pixel-aspect-ratio", GST_TYPE_FRACTION, 1, 1,
     "width", G_TYPE_INT, 512,
     "height", G_TYPE_INT, 512,
     NULL);

    ASSERT_TRUE(gst_element_link_filtered(src,transform,caps));
}