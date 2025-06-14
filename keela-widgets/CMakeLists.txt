
set(RESOURCE_XML "${CMAKE_CURRENT_SOURCE_DIR}/shader-resources.xml")
set(SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")
set(FRAGMENT_SRC "${SHADER_DIR}/hello-triangle-fragment.glsl")
set(VERTEX_SRC "${SHADER_DIR}/hello-triangle-vertex.glsl")
set(SHADER_C "${CMAKE_CURRENT_SOURCE_DIR}/shader-resources.c")
set(SHADER_H "${CMAKE_CURRENT_SOURCE_DIR}/keela-widgets/shader-resources.h")

add_custom_command(
        OUTPUT ${SHADER_C} ${SHADER_H}
        COMMAND glib-compile-resources ${RESOURCE_XML} --generate-source --sourcedir ${SHADER_DIR}
        COMMAND glib-compile-resources ${RESOURCE_XML} --generate-header --sourcedir ${SHADER_DIR} --target ${SHADER_H}
        DEPENDS ${RESOURCE_XML} ${FRAGMENT_SRC} ${VERTEX_SRC}
)

add_library(keela-widgets labeledspinbutton.cpp keela-widgets/labeledspinbutton.h
        framebox.cpp
        keela-widgets/framebox.h
        labeledcomboboxtext.cpp
        keela-widgets/labeledcomboboxtext.h
        GLCameraRender.cpp
        keela-widgets/GLCameraRender.h)

target_link_libraries(keela-widgets PUBLIC PkgConfig::GTKMM PkgConfig::GIO vendor-glad)
target_include_directories(keela-widgets PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})