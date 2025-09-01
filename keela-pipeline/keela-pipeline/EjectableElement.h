//
// Created by brand on 9/1/2025.
//

#ifndef EJECTBIN_H
#define EJECTBIN_H
#include "bin.h"
#include "elementbase.h"


namespace Keela {
    /**
     * A pipeline element that may be removed from a pipeline during live playback
     */
    class EjectableElement {
    public:
        /**
         * Prepare the element for ejection from the pipeline, but do not wait for the element to finish processing remaining data.
         */
        void PrepareEject();


        /**
         * Eject the element from the pipeline and wait for it to finish processing remaining data.
         */
        void Eject();

    private:
        /**
         * Get a pointer to the first element in the bin. Subclasses must be able to guarantee that they own the element pointer
         *
         * @return
         */
        virtual GstElement *Head() = 0;


        /**
         * get a pointer to the last element in the bin. Subclass must be able to guarantee that they own the element pointer
         * @return
         */
        virtual GstElement *Tail() = 0;

        /**
         * Hacky way to avoid ambiguity of conversion of this to GstElement* since we're using multiple inheritance
         */
        virtual GstElement *Element() = 0;

        /// callback unlinks element, sends EOS on its src, then installs an EOS event callback to clean up the remaining data
        static GstPadProbeReturn pad_block_callback(GstPad *pad, GstPadProbeInfo *info, gpointer user_data);

        /// callback cleans up any remaining data from the bin and removes it from the pipeline
        static GstPadProbeReturn event_callback(GstPad *pad, GstPadProbeInfo *info, gpointer user_data);

        bool safe_to_remove = false;
        std::mutex remove_mutex;
        std::condition_variable remove_condition;
    };
}


#endif //EJECTBIN_H
