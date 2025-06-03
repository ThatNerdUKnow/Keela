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
    Keela::Bin bin("bin");
    GstBin* b = bin;
    ASSERT_TRUE(GST_IS_BIN(b));
}

TEST(KeelaPipeline, UseBinAsGstElement) {
    Keela::Bin bin("bin");
    GstBin* b = bin;
    ASSERT_TRUE(GST_IS_ELEMENT(b));

}
#pragma endregion

TEST(KeelaPipeline, DuplicateNamedBins) {
    // WARN: this is only valid if the two bins are not added to the same parent bin
    // which at runtime I believe should never happen as each camera should be getting a unique name
    auto bin1 = Keela::Bin("Foo");
    auto bin2 = Keela::Bin("Foo");
}

TEST(KeelaPipeline, LinkBins) {
    auto bin1 = Keela::TransformBin();
    auto bin2 = Keela::RecordBin();
    ASSERT_TRUE(gst_element_link(bin1,bin2));
}

TEST(KeelaPipeline, ConstructTestSrc) {
    auto src = Keela::TestSrc();
}

TEST(KeelaPipeline, ConstructNamedTestSrc) {
    auto src = Keela::TestSrc("Foo");
}

TEST(KeelaPipeline, CanPlay) {
    Keela::Bin bin("Playbin");
    Keela::TestSrc src("TestSrc0");
    Keela::TransformBin transform("TransformBin");
    Keela::PresentationBin presentation("PresentationBin");
    spdlog::info("Created all elements");
    GstBin* b = bin;
    GstElement* s = src;
    GstElement* t = transform;
    GstElement* p = presentation;
    gst_bin_add_many(b,s,t,p,nullptr); // which causes this C function to fail
    ASSERT_TRUE(gst_element_link_many(s,t,p,nullptr));
    // TODO: link elements and set pipeline to playing
}