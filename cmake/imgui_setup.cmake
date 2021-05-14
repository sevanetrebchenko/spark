# Set Sources and Header files.
set(SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/lib/imgui/imgui.cpp"
        "${PROJECT_SOURCE_DIR}/lib/imgui/imgui_draw.cpp"
        "${PROJECT_SOURCE_DIR}/lib/imgui/imgui_widgets.cpp"
        "${PROJECT_SOURCE_DIR}/lib/imgui/imgui_demo.cpp"
        "${PROJECT_SOURCE_DIR}/lib/imgui/misc/cpp/imgui_stdlib.cpp"
    )

# TODO: platform specific files.
# OpenGL3 / GLFW Imgui sources
set(SOURCE_FILES
        ${SOURCE_FILES}
        "${PROJECT_SOURCE_DIR}/lib/imgui/examples/imgui_impl_opengl3.cpp"
        "${PROJECT_SOURCE_DIR}/lib/imgui/examples/imgui_impl_glfw.cpp"
    )

set(IMGUI_INCLUDE_DIRS
        "${PROJECT_SOURCE_DIR}/lib/imgui"
    )

# Add ImGui as a library.
add_library(spark-imgui STATIC "${SOURCE_FILES}")

# Set includes for targets using library.
target_include_directories(spark-imgui PUBLIC "${IMGUI_INCLUDE_DIRS}")
target_link_libraries(spark-imgui spark-glad) # Link Glad
target_link_libraries(spark-imgui glfw)       # Link GLFW

# Use Glad for ImGui.
target_compile_definitions(spark-imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)



