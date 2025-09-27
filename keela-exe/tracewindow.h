//
// Created by brand on 6/30/2025.
//

#ifndef TRACEWINDOW_H
#define TRACEWINDOW_H
#include <gtkmm/box.h>
#include <gtkmm/window.h>

#include "keela-widgets/GLTraceRender.h"


namespace Keela {
    class TraceWindow final : public Gtk::Window {
        // TODO: sync GLTraceRender::queue_draw across every instance of GLTraceRender to avoid jittering
    public:
        TraceWindow();

        ~TraceWindow() override;

        void addTraces(const std::vector<std::shared_ptr<Keela::ITraceable>>& traces);

        void removeTrace();

        int num_traces() const;

    private:
        std::vector<std::shared_ptr<GLTraceRender> > traces;
        Gtk::ScrolledWindow scrolled_window;
        Gtk::Box container = Gtk::Box(Gtk::ORIENTATION_VERTICAL);

        // sync up queueing draw events for trace widgets
        bool on_timeout();
    };
}

#endif //TRACEWINDOW_H
