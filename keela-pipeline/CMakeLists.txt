add_library(keela-pipeline presentationbin.cpp keela-pipeline/presentationbin.h
        recordbin.cpp
        keela-pipeline/recordbin.h
        TransformBin.cpp
        keela-pipeline/TransformBin.h
        bin.cpp
        keela-pipeline/bin.h
        keela-pipeline/gst-helpers.h
        gst-helpers.cpp
        queuebin.cpp
        keela-pipeline/queuebin.h
        element.cpp
        keela-pipeline/element.h
        testsrc.cpp
        keela-pipeline/testsrc.h
)
target_link_libraries(keela-pipeline PUBLIC PkgConfig::GSTREAMER PkgConfig::SPDLOG)

target_include_directories(keela-pipeline PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})