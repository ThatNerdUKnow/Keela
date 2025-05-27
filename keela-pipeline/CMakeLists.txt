add_library(keela-pipeline presentationbin.cpp keela-pipeline/presentationbin.h
        recordbin.cpp
        keela-pipeline/recordbin.h
        commonbin.cpp
        keela-pipeline/commonbin.h
        bin.cpp
        keela-pipeline/bin.h
)
target_link_libraries(keela-pipeline PUBLIC PkgConfig::GSTREAMER)