//
// Created by brand on 5/31/2025.
//

#ifndef AUTOVIDEOSINK_H
#define AUTOVIDEOSINK_H
#include "simpleelement.h"

namespace Keela {
    class Autovideosink final : public SimpleElement {
        public:
        Autovideosink(): SimpleElement("Autovideosink") {};
        explicit Autovideosink(const std::string &name): SimpleElement("AutoVideoSink",name) {};
    };
}
#endif //AUTOVIDEOSINK_H
