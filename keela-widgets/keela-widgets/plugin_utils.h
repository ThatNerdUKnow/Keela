//
// Helper functions for using custom Keela plugins
//

#ifndef KEELA_PLUGIN_UTILS_H
#define KEELA_PLUGIN_UTILS_H

#include <gstreamer-1.0/gst/gst.h>

#include <string>

namespace Keela {
    /**
     * Check if the custom keelavideotestsrc plugin is available
     */
    bool is_custom_videotestsrc_available();

    /**
     * Get the appropriate video test source element name
     * Returns "keelavideotestsrc" if available, otherwise "videotestsrc"
     */
    std::string get_video_test_source_name();
}  // namespace Keela

#endif  // KEELA_PLUGIN_UTILS_H
