add_executable(keela main.cpp mainwindow.cpp mainwindow.h)

target_link_libraries(keela PRIVATE keela-widgets keela-pipeline PkgConfig::GSTREAMER PkgConfig::GTKMM)