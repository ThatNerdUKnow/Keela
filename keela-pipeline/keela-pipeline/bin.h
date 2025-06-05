//
// Created by brand on 5/26/2025.
//

#ifndef BIN_H
#define BIN_H
#include <memory>
#include <stdexcept>
#include <string>
#include <gstreamer-1.0/gst/gst.h>
#include <spdlog/spdlog.h>

#include "elementbase.h"

namespace Keela {
    class Bin: public Keela::Element {
        public:
        explicit Bin(const std::string &name);
        //Bin(const Bin &bin);
        Bin();

        ~Bin() override;



        operator GstElement*() const override;

        /**
         * use this function to add many elements to the bin. If we use `gst_bin_add_many` directly,
         * temporaries will be created using the implicit copy constructor which will mess up the underlying
         * `GstElement` refcount.
         * @tparam First any type which can convert to GstElement*
         */
        template <typename First, typename ...Rest>
        void add_elements(First first, Rest... rest) {
            GstElement* e = first;
            GstElement* b = *this;
            gst_bin_add(GST_BIN(b), GST_ELEMENT(e));
            // gst_bin_add is void, so this is how we check if the element *actually* got added
            auto parent = gst_element_get_parent(e);

            if (GST_ELEMENT(parent) != b) {
                throw std::runtime_error("could not add element to bin");
            }
            // gst_element_get_parent will ref this bin, so keep our ref count in check
            g_object_unref(parent);
            add_elements(rest...);
        }

        static void add_elements() {
            spdlog::info("{} Added all elements to bin",__func__);
        };


    protected:
        std::shared_ptr<GstBin*> bin;
        //GstBin *bin;

        /**
         * Create a ghost pad for an internal GstElement
         */
        void add_ghost_pad(GstElement* element, const std::string &pad_name) const;

    private:
        /* Create required elements and add them to the bin
         */
        virtual void init() {};

        /* Link elements together
         */
        virtual void link() {}
    };
}
#endif //BIN_H
