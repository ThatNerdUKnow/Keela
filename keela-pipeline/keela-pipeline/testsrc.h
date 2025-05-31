//
// Created by brand on 5/30/2025.
//

#ifndef TESTSRC_H
#define TESTSRC_H
#include "simpleelement.h"

namespace Keela {
    class TestSrc: public Keela::SimpleElement {
        public:
        TestSrc();
        explicit TestSrc(const std::string& name);
    };
}
#endif //TESTSRC_H
