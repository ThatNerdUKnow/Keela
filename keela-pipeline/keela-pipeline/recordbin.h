//
// Created by brand on 5/26/2025.
//

#ifndef RECORDBIN_H
#define RECORDBIN_H
#include "bin.h"
#include "queuebin.h"
#include "simpleelement.h"

namespace Keela {
    class RecordBin final : public QueueBin, public EjectableElement {
    public:
        explicit RecordBin(const std::string &name);


        RecordBin();

        void set_directory(const std::string &full_filename);

        ~RecordBin() override;

        Keela::SimpleElement enc = SimpleElement("x264enc");
        Keela::SimpleElement mux = SimpleElement("matroskamux");
        Keela::SimpleElement sink = SimpleElement("filesink");


        // these are used to control safe removal of this element from the pipeline

        bool safe_to_remove = false;
        std::mutex remove_mutex;
        std::condition_variable remove_condition;

    private:
        void link() override;

        void init() override;

        GstElement *Head() override {
            return queue;
        };

        GstElement *Tail() override {
            return mux;
        }

        GstElement *Element() override {
            return *this;
        };;

        std::string name;
    };
}
#endif //RECORDBIN_H
