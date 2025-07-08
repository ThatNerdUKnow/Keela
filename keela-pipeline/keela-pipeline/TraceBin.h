//
// Created by brand on 7/7/2025.
//

#ifndef TRACEBIN_H
#define TRACEBIN_H
#include "queuebin.h"

namespace Keela {
    class TraceBin : public QueueBin {
    public:
        TraceBin();

        explicit TraceBin(std::string name);

        ~TraceBin() override;

        /*
         * Whether to enable tracing or not
         */
        void enable_trace(bool do_enable);

    private:
        void init() override;

        void link() override;

    public:
        SimpleElement sink = SimpleElement("appsink");
    };
}
#endif //TRACEBIN_H
