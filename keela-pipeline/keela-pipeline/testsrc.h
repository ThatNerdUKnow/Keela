//
// Created by brand on 5/30/2025.
//

#ifndef TESTSRC_H
#define TESTSRC_H
#include "element.h"

namespace Keela {
    class TestSrc: public Keela::Element {
        public:
        TestSrc();
        explicit TestSrc(const std::string& name);
    };
}
#endif //TESTSRC_H
