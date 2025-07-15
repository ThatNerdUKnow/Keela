//
// Created by brand on 6/10/2025.
//

#ifndef SNAPSHOTBIN_H
#define SNAPSHOTBIN_H
#include "bin.h"
#include "queuebin.h"
#include "simpleelement.h"

namespace Keela {
    class SnapshotBin final : public Keela::QueueBin {
    public:
        SnapshotBin();

        ~SnapshotBin() override;

        void take_snapshot();

    private:
        SimpleElement app_sink = SimpleElement("appsink");

        void init() override;

        void link() override;
    };
}
#endif //SNAPSHOTBIN_H
