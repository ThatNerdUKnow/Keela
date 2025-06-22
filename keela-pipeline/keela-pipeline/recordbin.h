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

        void set_directory(const std::string &full_filename);

        ~RecordBin() override;

        // use multifilesink to not overwrite any data if user forgets to switch directories
        Keela::SimpleElement sink = SimpleElement("filesink");

    private:
        void link() override;

        void init() override;

        std::string name;
        Keela::SimpleElement enc = SimpleElement("x264enc");
        Keela::SimpleElement mux = SimpleElement("matroskamux");
    };
}
#endif //RECORDBIN_H
