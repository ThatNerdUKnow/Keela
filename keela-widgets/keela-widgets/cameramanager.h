//
// Created by brand on 6/3/2025.
//

#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H
#include <set>
#include <keela-pipeline/bin.h>
#include <keela-pipeline/simpleelement.h>
#include <keela-pipeline/caps.h>
#include <keela-pipeline/presentationbin.h>
#include <keela-pipeline/transformbin.h>

#include <keela-pipeline/recordbin.h>
#include <keela-pipeline/snapshotbin.h>

#include "keela-pipeline/TraceBin.h"

namespace Keela {
    class CameraManager final : public Keela::Bin {
    public:
        explicit CameraManager(guint id, bool split_streams);

        ~CameraManager() override;

        void set_framerate(double framerate);

        void set_resolution(int width, int height);

        void set_experiment_directory(const std::string &path);

        void start_recording();

        void stop_recording();


        SimpleElement camera;
        SimpleElement caps_filter = SimpleElement("capsfilter");
        TransformBin transform = TransformBin("transform");
        std::shared_ptr<PresentationBin> presentation = std::make_shared<PresentationBin>("presentation");
        SnapshotBin snapshot;
        std::shared_ptr<TraceBin> trace = std::make_shared<TraceBin>("trace");

    private:
        guint id;
        bool split_streams;

        /// caps filter to apply to the entire stream
        Caps base_caps;

        /// caps filter determining the stream caps after scaling
        Caps scaled_caps;

        /// for now, experiment directory will be set to my temp directory until I figure out gtk file dialogs
        std::string experiment_directory = "C:\\temp";

        /**
         * use to split a stream into as many identical streams as we want.
         *
         * NOTE: any elements that come after "tee" should probably inherit from Keela::QueueBin
         */
        SimpleElement tee = SimpleElement("tee");

        /**
         * use to convert video color space to GRAY8 if camera does not support it
         */
        SimpleElement auto_video_convert = SimpleElement("videoconvert");

        /* at any moment there may be many active record bins
         *
         * TODO: do these still need to be shared_ptr?
         *
         */
        std::set<std::shared_ptr<RecordBin> > record_bins;

        /*
         * prepends the filename with the current time to avoid overwriting files
         * ex: directory/20250915_181211_cam_1.mkv
         * supports cross-platform path joining
         */
        static std::string get_filename(std::string directory, guint cam_id);
    };
}
#endif //CAMERAMANAGER_H
