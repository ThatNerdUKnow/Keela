//
// Created by brand on 5/28/2025.
//

#ifndef QUEUEBIN_H
#define QUEUEBIN_H
#include <keela-pipeline/bin.h>
namespace Keela {
    class QueueBin: public Bin {
        public:
        QueueBin();
        explicit QueueBin(const std::string &name);
        protected:
        void link_queue(GstElement* sink) const;

    private:
        void init() override;

        void link() override;

        GstElement* queue = nullptr;
    };
}
#endif //QUEUEBIN_H
