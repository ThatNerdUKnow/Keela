//
// Created by brand on 5/30/2025.
//

#include "keela-pipeline/testsrc.h"
#define TEST_SRC_ELEMENT_NAME "videotestsrc"

Keela::TestSrc::TestSrc():Element(TEST_SRC_ELEMENT_NAME) {
}

Keela::TestSrc::TestSrc(const std::string& name):Element(TEST_SRC_ELEMENT_NAME,name) {
}
