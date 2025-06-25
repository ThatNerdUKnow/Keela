
set(RESOURCE_XML "${CMAKE_CURRENT_SOURCE_DIR}/shader-resources.xml")
set(SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")
set(SHADER_C "${CMAKE_CURRENT_SOURCE_DIR}/shader-resources.c")
set(SHADER_H "${CMAKE_CURRENT_SOURCE_DIR}/keela-widgets/shader-resources_raw.h")
set(SHADER_DEPFILE "${CMAKE_CURRENT_SOURCE_DIR}/shader-resources-depfile")


add_custom_target(shader_resources DEPENDS ${SHADER_C} ${SHADER_H} ${SHADER_DEPFILE})
add_custom_command(
        OUTPUT ${SHADER_C} ${SHADER_H} ${SHADER_DEPFILE}
        COMMAND glib-compile-resources ${RESOURCE_XML} --generate-source --sourcedir ${SHADER_DIR} --c-name=shader_resources --dependency-file=${SHADER_DEPFILE}
        COMMAND glib-compile-resources ${RESOURCE_XML} --generate-header --sourcedir ${SHADER_DIR} --target ${SHADER_H} --c-name=shader_resources
        DEPFILE ${SHADER_DEPFILE}
)


add_library(keela-widgets labeledspinbutton.cpp keela-widgets/labeledspinbutton.h
        framebox.cpp
        keela-widgets/framebox.h
        labeledcomboboxtext.cpp
        keela-widgets/labeledcomboboxtext.h
        GLCameraRender.cpp
        keela-widgets/GLCameraRender.h ${SHADER_C} ${SHADER_H}
        keela-widgets/shader-resources.h
        tracegizmo.cpp
        keela-widgets/tracegizmo.h)
add_dependencies(keela-widgets shader_resources)
target_link_libraries(keela-widgets PUBLIC keela-pipeline PkgConfig::GTKMM PkgConfig::GIO PkgConfig::SPDLOG vendor-glad)
target_include_directories(keela-widgets PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})