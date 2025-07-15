CMAKE_MINIMUM_REQUIRED(VERSION 3.28)
project(Keela)
find_package(PkgConfig)

# GoogleTest requires at least C++17
# We need C++20 for jthreads
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wall)

option(MAKE_REFERENCE "Build reference gstreamer gtk integration" false)
pkg_search_module(GTKMM REQUIRED IMPORTED_TARGET gtkmm-3.0)
pkg_search_module(GSTREAMER REQUIRED IMPORTED_TARGET gstreamer-1.0)
pkg_search_module(SPDLOG REQUIRED IMPORTED_TARGET spdlog)
pkg_search_module(GIO REQUIRED IMPORTED_TARGET gio-2.0)
add_subdirectory(vendor)
add_subdirectory(keela-widgets)
add_subdirectory(keela-pipeline)
add_subdirectory(keela-exe)
add_subdirectory(keela-test)
add_subdirectory(reference)