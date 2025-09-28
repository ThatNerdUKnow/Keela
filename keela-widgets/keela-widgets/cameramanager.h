//
// Created by brand on 6/3/2025.
//

#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H
#include <keela-pipeline/CameraStreamBin.h>
#include <keela-pipeline/bin.h>
#include <keela-pipeline/caps.h>
#include <keela-pipeline/presentationbin.h>
#include <keela-pipeline/recordbin.h>
#include <keela-pipeline/simpleelement.h>
#include <keela-pipeline/snapshotbin.h>
#include <keela-pipeline/transformbin.h>

#include <atomic>
#include <set>

#include "keela-pipeline/TraceBin.h"

namespace Keela {
    class CameraManager final : public Keela::Bin {
    public:
        explicit CameraManager(guint id, std::string pix_fmt, bool split_streams);

        ~CameraManager() override;

        void set_pix_fmt(const std::string &format);

        void set_framerate(double framerate);

        void set_resolution(int width, int height);

        void set_experiment_directory(const std::string &path);

        void start_recording();

        void stop_recording();

        // Control frame splitting
        void set_frame_splitting(bool enabled);

        bool is_frame_splitting_enabled() const { return split_streams; }

        // Camera Streams manage presentation, recording, and tracing of their respective frame streams
        std::shared_ptr<CameraStreamBin> camera_stream_even = std::make_shared<CameraStreamBin>("camera_stream_even");
        std::shared_ptr<CameraStreamBin> camera_stream_odd = std::make_shared<CameraStreamBin>("camera_stream_odd");

        SimpleElement camera = SimpleElement("videotestsrc");
        SimpleElement caps_filter = SimpleElement("capsfilter");
        TransformBin transform = TransformBin("transform");


    private:
        void set_up_frame_splitting();

        void install_frame_splitting_probes();

        // Frame filtering callbacks
        static GstPadProbeReturn frame_numbering_probe_cb(GstPad *pad, GstPadProbeInfo *info, gpointer user_data);

        static GstPadProbeReturn even_frame_probe_cb(GstPad *pad, GstPadProbeInfo *info, gpointer user_data);

        static GstPadProbeReturn odd_frame_probe_cb(GstPad *pad, GstPadProbeInfo *info, gpointer user_data);

        guint id;
        bool split_streams;

        // Frame counting for even/odd filtering
        std::atomic<guint64> frame_count{0};

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
        SimpleElement tee_main = SimpleElement("tee");

        /* at any moment there may be many active record bins
         *
         * TODO: do these still need to be shared_ptr?
         *
         */
        std::set<std::shared_ptr<RecordBin> > record_bins;

        /*
         * prepends the filename with the current time to avoid overwriting files
         * ex: directory/20250915_181211_cam_1.mkv
         *     directory/20250915_181211_cam_1_even.mkv (if frame splitting is enabled)
         * supports cross-platform path joining
         */
        static std::string get_filename(std::string directory, guint cam_id, std::string suffix = "");

        void add_odd_camera_stream();
    };
} // namespace Keela
#endif  // CAMERAMANAGER_H
