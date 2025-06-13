add_library(keela-widgets labeledspinbutton.cpp keela-widgets/labeledspinbutton.h
        framebox.cpp
        keela-widgets/framebox.h
        labeledcomboboxtext.cpp
        keela-widgets/labeledcomboboxtext.h
        GLCameraRender.cpp
        keela-widgets/GLCameraRender.h)

target_link_libraries(keela-widgets PUBLIC PkgConfig::GTKMM vendor-glad)
target_include_directories(keela-widgets PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})