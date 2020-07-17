# Set Sources and Header files.
set(SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/lib/imgui/imgui.cpp"
        "${PROJECT_SOURCE_DIR}/lib/imgui/imgui_draw.cpp"
        "${PROJECT_SOURCE_DIR}/lib/imgui/imgui_widgets.cpp"
        "${PROJECT_SOURCE_DIR}/lib/imgui/imgui_demo.cpp")

set(INCLUDE_DIRS
        "${PROJECT_SOURCE_DIR}/lib/imgui")

# Add ImGui as a library.
add_library(spark-imgui STATIC "${SOURCE_FILES}")

# Set includes for targets using library.
target_include_directories(spark-imgui PUBLIC "${INCLUDE_DIRS}")

