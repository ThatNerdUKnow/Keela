add_executable(keela main.cpp mainwindow.cpp mainwindow.h)

target_link_libraries(keela PRIVATE PkgConfig::GSTREAMER PkgConfig::GTKMM)