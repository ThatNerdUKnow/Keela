

if (MAKE_REFERENCE)
    pkg_search_module(GTK3 REQUIRED IMPORTED_TARGET gtk+-3.0)
    add_executable(gst-tutorial-5 gst-tutorial-5.c)
    target_link_libraries(gst-tutorial-5 PRIVATE PkgConfig::GSTREAMER PkgConfig::GTK3)

    #add_executable(gst_jpg_on_demand gst_jpg_on_demand.cpp)
    #target_link_libraries(gst_jpg_on_demand PRIVATE PkgConfig::GSTREAMER)
else ()
    message("Skipping gstreamer gui toolkit reference implementation")
endif ()