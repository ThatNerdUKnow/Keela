//
// Created by brand on 5/31/2025.
//

#ifndef AUTOVIDEOSINK_H
#define AUTOVIDEOSINK_H
#include "simpleelement.h"

namespace Keela {
    class Autovideosink final : public SimpleElement {
        public:
        Autovideosink(): SimpleElement("autovideosink") {};
        explicit Autovideosink(const std::string &name): SimpleElement("autovideosink",name) {};
    };
}
#endif //AUTOVIDEOSINK_H
