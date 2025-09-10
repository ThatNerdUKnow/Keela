set(PLUGIN_INSTALL_DIR ${CMAKE_BINARY_DIR}/plugins)

# Ensure filename matches GStreamer autoloader pattern: libgst*.so
set_target_properties(gstkeelavideotestsrc PROPERTIES
    OUTPUT_NAME "keelavideotestsrc"
    PREFIX "lib"
)

# Define required macros
target_compile_options(gstkeelavideotestsrc PRIVATE ${GST_CFLAGS})
target_compile_definitions(gstkeelavideotestsrc PRIVATE
    PACKAGE="keela-videotestsrc"
    VERSION="1.0"
    PACKAGE_VERSION="1.0"
    GST_PACKAGE_NAME="Keela videotestsrc"
    GST_PACKAGE_ORIGIN="https://github.com/Evan-Hartley/Keela"
)

install(TARGETS gstkeelavideotestsrc
    LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
)

add_custom_command(TARGET gstkeelavideotestsrc POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PLUGIN_INSTALL_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:gstkeelavideotestsrc> ${PLUGIN_INSTALL_DIR}/
)

add_library(gstkeelavideotestsrc SHARED
    gstkeelavideotestsrc.c
    gstkeelacustomplugin.c
)
target_link_libraries(gstkeelavideotestsrc ${GST_LIBRARIES} m )
target_include_directories(gstkeelavideotestsrc PRIVATE ${GST_INCLUDE_DIRS})
