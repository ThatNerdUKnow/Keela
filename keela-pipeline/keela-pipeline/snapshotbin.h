//
// Created by brand on 6/10/2025.
//

#ifndef SNAPSHOTBIN_H
#define SNAPSHOTBIN_H
#include "bin.h"
#include "simpleelement.h"

namespace Keela {
    class SnapshotBin : public Keela::Bin {
    public:
        SnapshotBin();

        ~SnapshotBin() override;

        void take_snapshot(std::string filename);

    private:
        SimpleElement app_sink = SimpleElement("appsink");
        SimpleElement app_source = SimpleElement("appsrc");
        SimpleElement file_sink = SimpleElement("filesink");

        void init() override;

        void link() override;
    };
}
#endif //SNAPSHOTBIN_H
