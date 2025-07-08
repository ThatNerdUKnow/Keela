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
    public:
        TraceWindow();

        virtual ~TraceWindow();

        void addTrace(std::shared_ptr<Keela::ITraceable> trace);

        void removeTrace();

        int num_traces() const;

        void set_framerate(double framerate);

    private:
        std::vector<std::shared_ptr<GLTraceRender> > traces;
        Gtk::ScrolledWindow scrolled_window;
        Gtk::Box containter = Gtk::Box(Gtk::ORIENTATION_VERTICAL);
    };
}

#endif //TRACEWINDOW_H
