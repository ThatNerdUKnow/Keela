//
// Created by brand on 5/26/2025.
//

#ifndef RECORDBIN_H
#define RECORDBIN_H
#include "bin.h"

namespace Keela {
    class RecordBin : public Bin {
        public:
        explicit RecordBin(const std::string &name);



        RecordBin();
        ~RecordBin();

        private:
        void init();
        std::string name;
        GstElement *enc = nullptr;
        GstElement *mux = nullptr;
        GstElement *sink = nullptr;
    };
}
#endif //RECORDBIN_H
