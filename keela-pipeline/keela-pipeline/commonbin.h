//
// Created by brand on 5/26/2025.
//

#ifndef COMMONBIN_H
#define COMMONBIN_H
#include "bin.h"

namespace Keela {
    class CommonBin: Keela::Bin {
        public:
        CommonBin();

        /**
         *
         * @param name Name of bin
         * @param subsample whether to subsample the video stream
         */
        explicit CommonBin(std::string name, bool subsample);
        ~CommonBin();
        private:
        bool do_subsample;
        GstElement *scale;
        GstElement* tee;
    };
}
#endif //COMMONBIN_H
