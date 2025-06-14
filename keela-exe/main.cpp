#include <gtkmm/application.h>

#include "mainwindow.h"
#include <keela-widgets/shader-resources.h>
//
// Created by brand on 5/25/2025.
//

int main(int argc, char **argv) {
    auto app = Gtk::Application::create(argc, argv, "com.gatech.keela");
    gst_init(&argc, &argv);
    MainWindow window;
    return app->run(window);
}
