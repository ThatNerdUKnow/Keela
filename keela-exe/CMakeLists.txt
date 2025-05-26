add_executable(keela main.cpp mainwindow.cpp mainwindow.h
        ../keela-widgets/labeledspinbutton.cpp
        ../keela-widgets/keela-widgets/labeledspinbutton.h
)

target_link_libraries(keela PRIVATE keela-widgets PkgConfig::GSTREAMER PkgConfig::GTKMM)