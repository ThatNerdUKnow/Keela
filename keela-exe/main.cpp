#include <gtkmm/application.h>

#include "mainwindow.h"
//
// Created by brand on 5/25/2025.
//
int main(int argc, char **argv) {
    spdlog::set_level(spdlog::level::trace);
    auto app = Gtk::Application::create(argc, argv, "com.gatech.keela");
    gst_init(&argc, &argv);
    MainWindow window;
    return app->run(window);
}
