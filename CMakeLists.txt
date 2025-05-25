CMAKE_MINIMUM_REQUIRED(3.31)
project(Keela)

find_package(PkgConfig)

pkg_search_module(GTKMM REQUIRED IMPORTED_TARGET gtkmm3)
pkg_search_module(GSTREAMER REQUIRED IMPORTED_TARGET gstreamer-1.0)
add_subdirectory(keela-exe)