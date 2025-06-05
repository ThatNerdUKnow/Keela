//
// Created by brand on 5/26/2025.
//

#ifndef RECORDBIN_H
#define RECORDBIN_H
#include "bin.h"
#include "queuebin.h"
#include "simpleelement.h"

namespace Keela {
    class RecordBin : public QueueBin {
    public:
        explicit RecordBin(const std::string &name);


        RecordBin();

        ~RecordBin() override;

    private:
        void link() override;

    private:
        std::string name;
        Keela::SimpleElement enc = SimpleElement("x264enc");
        Keela::SimpleElement mux = SimpleElement("mp4mux");
        Keela::SimpleElement sink = SimpleElement("filesink");
    };
}
#endif //RECORDBIN_H
