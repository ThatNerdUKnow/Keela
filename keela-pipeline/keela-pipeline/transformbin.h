//
// Created by brand on 5/26/2025.
//

#ifndef COMMONBIN_H
#define COMMONBIN_H
#include "simpleelement.h"
#include "keela-pipeline/bin.h"

namespace Keela {
    class TransformBin : public Bin {
    public:
        TransformBin();

        /**
         *
         * @param name Name of bin
         * @param subsample whether to subsample the video stream
         */
        explicit TransformBin(const std::string &name);

        ~TransformBin();

    private:
        void init() override;

        void link() override;

        Keela::SimpleElement scale = SimpleElement("videoscale");
        //GstElement *scale = nullptr;
    };
}
#endif //COMMONBIN_H
