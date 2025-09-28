#ifndef CAMERASTREAMBIN_H
#define CAMERASTREAMBIN_H

#include <keela-pipeline/presentationbin.h>
#include <keela-pipeline/snapshotbin.h>

#include "EjectableElement.h"
#include "caps.h"
#include "keela-pipeline/TraceBin.h"
#include "queuebin.h"
#include "simpleelement.h"

namespace Keela {
    class CameraStreamBin final : public QueueBin, public EjectableElement {
    public:
        explicit CameraStreamBin(const std::string &name);

        ~CameraStreamBin() override;

        std::shared_ptr<PresentationBin> presentation;
        std::shared_ptr<SnapshotBin> snapshot;
        std::shared_ptr<TraceBin> trace;

    private:
        void init() override;
        void link() override;

        Keela::Element *Head() override {
            return &queue;
        };

        std::vector<Keela::Element *> Leaves() override {
            std::vector<Keela::Element *> ret;
            // @TODO: return any record_bins
            return ret;
        }

        std::string name;
        SimpleElement internal_tee = SimpleElement("tee");
    };
}  // namespace Keela
#endif  // CAMERASTREAMBIN_H
