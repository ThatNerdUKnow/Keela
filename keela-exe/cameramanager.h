//
// Created by brand on 6/3/2025.
//

#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H
#include <keela-pipeline/bin.h>
#include <keela-pipeline/simpleelement.h>
#include <keela-pipeline/caps.h>
#include <keela-pipeline/presentationbin.h>
#include <keela-pipeline/transformbin.h>

#include "keela-pipeline/snapshotbin.h"

namespace Keela {
    class CameraManager final : public Keela::Bin {
    public:
        explicit CameraManager(guint id, bool split_streams);

        ~CameraManager() override;

        void set_framerate(double framerate);

        void set_resolution(int width, int height);

        SimpleElement camera;
        SimpleElement caps_filter = SimpleElement("capsfilter");
        TransformBin transform = TransformBin("transform");
        PresentationBin presentation = PresentationBin("presentation");
        SnapshotBin snapshot;

    private:
        guint id;
        bool split_streams;

        // caps filter to apply to the entire stream
        Caps base_caps;

        // caps filter determining the stream caps after scaling
        Caps scaled_caps;

        SimpleElement tee = SimpleElement("tee");
        SimpleElement auto_video_convert = SimpleElement("autovideoconvert");
    };
}
#endif //CAMERAMANAGER_H
