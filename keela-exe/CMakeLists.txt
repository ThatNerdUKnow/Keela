add_executable(keela main.cpp)

target_link_libraries(keela PRIVATE PkgConfig::GSTREAMER PkgConfig::GTKMM)