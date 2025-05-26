add_library(keela-widgets labeledspinbutton.cpp keela-widgets/labeledspinbutton.h)

target_link_libraries(keela-widgets PUBLIC PkgConfig::GTKMM)
target_include_directories(keela-widgets PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})