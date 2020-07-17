# Initialize dependencies.

# Build glad.
add_subdirectory(glad)

# Build GLFW.
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build GLFW documentation." FORCE)     # Turn off building GLFW documentation.
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build GLFW tests." FORCE)    # Turn off building GLFW tests.
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build GLFW examples." FORCE) # Turn off building GLFW example code.
add_subdirectory(glfw)

# Build ImGui.
include(imgui_setup
        RESULT_VARIABLE IMGUI_SETUP_RESULT)
if(IMGUI_SETUP_RESULT STREQUAL "NOTFOUND")
    message(FATAL_ERROR "CMake module 'imgui_setup' was not found. ImGui was not built correctly.")
endif()

