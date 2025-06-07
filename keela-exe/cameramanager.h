//
// Created by brand on 6/3/2025.
//

#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H
#include <keela-pipeline/bin.h>

#include <keela-pipeline/simpleelement.h>

#include "keela-pipeline/presentationbin.h"
#include "keela-pipeline/transformbin.h"

namespace Keela {
    class CameraManager final : public Keela::Bin {
    public:
        explicit CameraManager(guint id, bool split_streams);

        ~CameraManager();

        void set_framerate(double framerate);

        void set_resolution(guint width, guint height);

        SimpleElement camera;
        SimpleElement caps_filter;
        TransformBin transform = TransformBin("transform");
        PresentationBin presentation = PresentationBin("presentation");

    private:
        guint id;
        bool split_streams;


        SimpleElement tee;
    };
}
#endif //CAMERAMANAGER_H
