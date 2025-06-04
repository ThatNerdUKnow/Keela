add_executable(keela main.cpp mainwindow.cpp mainwindow.h
        cameracontrolwindow.cpp
        cameracontrolwindow.h)

# creating seperate keela library so that they can be used in gtest
add_library(libkeela cameramanager.cpp cameramanager.h)
target_link_libraries(libkeela PUBLIC keela-pipeline PkgConfig::GSTREAMER PkgConfig::GTKMM PkgConfig::SPDLOG)

target_link_libraries(keela PRIVATE libkeela keela-widgets keela-pipeline PkgConfig::GSTREAMER PkgConfig::GTKMM PkgConfig::SPDLOG)