//
// Helper functions for using custom Keela plugins
//

#include "keela-widgets/plugin_utils.h"

#include <spdlog/spdlog.h>

namespace Keela {

    bool is_custom_videotestsrc_available() {
        GstElementFactory* factory = gst_element_factory_find("keelavideotestsrc");
        if (factory) {
            gst_object_unref(factory);
            return true;
        }
        return false;
    }

    std::string get_video_test_source_name() {
#ifdef KEELA_VIDEOTESTSRC_ENABLED
        if (is_custom_videotestsrc_available()) {
            spdlog::info("Using custom keelavideotestsrc plugin");
            return "keelavideotestsrc";
        } else {
            spdlog::info("Custom videotestsrc enabled but not found - using standard videotestsrc plugin");
            return "videotestsrc";
        }
#else
        spdlog::info("Custom videotestsrc disabled at build time - using standard videotestsrc plugin");
        return "videotestsrc";
#endif
    }
}  // namespace Keela
