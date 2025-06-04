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
    private:

        guint id;
        bool split_streams;

        SimpleElement camera;
        SimpleElement caps_filter;
        TransformBin transform;
        SimpleElement tee;
        PresentationBin presentation;
    };
}
#endif //CAMERAMANAGER_H
