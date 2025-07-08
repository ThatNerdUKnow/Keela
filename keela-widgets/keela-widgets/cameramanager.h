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
        TraceBin trace;

    private:
        guint id;
        bool split_streams;

        // caps filter to apply to the entire stream
        Caps base_caps;

        // caps filter determining the stream caps after scaling
        Caps scaled_caps;

        // for now, experiment directory will be set to my temp directory until I figure out gtk file dialogs
        std::string experiment_directory = "C:\\temp";

        SimpleElement tee = SimpleElement("tee");
        SimpleElement auto_video_convert = SimpleElement("videoconvert");

        /// at any moment there may be many active record bins
        std::set<std::shared_ptr<RecordBin> > record_bins;

        /// callback unlinks recordbin, sends EOS on its src, then installs an EOS event callback to clean up the remaining data
        static GstPadProbeReturn pad_block_callback(GstPad *pad, GstPadProbeInfo *info, gpointer user_data);

        /// callback cleans up any remaining data from the recordbin and removes it from the pipeline
        static GstPadProbeReturn event_callback(GstPad *pad, GstPadProbeInfo *info, gpointer user_data);
    };
}
#endif //CAMERAMANAGER_H
