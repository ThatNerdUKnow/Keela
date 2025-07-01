//
// Created by brand on 6/30/2025.
//

#ifndef GLTRACERENDER_H
#define GLTRACERENDER_H
#include <gtkmm/box.h>
#include <gtkmm/glarea.h>
#include <gtkmm/label.h>

namespace Keela {
    class GLTraceRender final : public Gtk::Box {
    public:
        GLTraceRender();

        virtual ~GLTraceRender();

    private:
        Gtk::GLArea gl_area;
        Gtk::Label label;
    };
}
#endif //GLTRACERENDER_H
