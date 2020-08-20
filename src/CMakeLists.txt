# PROJECT FILES
# TODO: Platform specific.
#file(GLOB_RECURSE
#        CORE_SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp"
#    )

set(CORE_SOURCE_FILES
        # Core
        "${PROJECT_SOURCE_DIR}/src/core/entry_point.cpp"
        "${PROJECT_SOURCE_DIR}/src/core/application.cpp"
        "${PROJECT_SOURCE_DIR}/src/core/layer.cpp"
        "${PROJECT_SOURCE_DIR}/src/core/layer_stack.cpp"
        "core/service_locator.cpp"
        # ECS
        "${PROJECT_SOURCE_DIR}/src/ecs/entities/entity_manager.cpp"
        "${PROJECT_SOURCE_DIR}/src/ecs/entities/entity_manager_data.cpp"
        # Events
        "${PROJECT_SOURCE_DIR}/src/events/event_hub.cpp"
        "${PROJECT_SOURCE_DIR}/src/events/null_event_hub.cpp"
        "${PROJECT_SOURCE_DIR}/src/events/event_listener.tpp"
        "${PROJECT_SOURCE_DIR}/src/events/application_events.cpp"
        "${PROJECT_SOURCE_DIR}/src/events/key_events.cpp"
        "${PROJECT_SOURCE_DIR}/src/events/mouse_events.cpp"
        "${PROJECT_SOURCE_DIR}/src/events/event.cpp"
        "${PROJECT_SOURCE_DIR}/src/events/event_interactable_interface.tpp"
        # Graphics
        "${PROJECT_SOURCE_DIR}/src/graphics/window/window.cpp"
        "${PROJECT_SOURCE_DIR}/src/graphics/context/rendering_context.cpp"
        "${PROJECT_SOURCE_DIR}/src/graphics/context/imgui_layer.cpp"
        "${PROJECT_SOURCE_DIR}/src/graphics/renderer/renderer_api.cpp"
        # Platform
        "${PROJECT_SOURCE_DIR}/src/platform/opengl/opengl_window.cpp"
        "${PROJECT_SOURCE_DIR}/src/platform/opengl/opengl_rendering_context.cpp"
        "${PROJECT_SOURCE_DIR}/src/platform/opengl/opengl_renderer_api.cpp"
        "${PROJECT_SOURCE_DIR}/src/platform/opengl/opengl_imgui_layer.cpp"
        # Logger
        "${PROJECT_SOURCE_DIR}/src/utilitybox/logger/adapter.cpp"
        "${PROJECT_SOURCE_DIR}/src/utilitybox/logger/adapter_config.cpp"
        "${PROJECT_SOURCE_DIR}/src/utilitybox/logger/log_message.cpp"
        "${PROJECT_SOURCE_DIR}/src/utilitybox/logger/logger.cpp"
        "${PROJECT_SOURCE_DIR}/src/utilitybox/logger/logging_system.cpp"
        "${PROJECT_SOURCE_DIR}/src/utilitybox/logger/invalid_format_character_exception.cpp"
        "${PROJECT_SOURCE_DIR}/src/utilitybox/logger/logging_interface.cpp"
        # Memory
        "${PROJECT_SOURCE_DIR}/src/utilitybox/memory/segmented_pool_allocator.cpp"
        "${PROJECT_SOURCE_DIR}/src/utilitybox/memory/memory_formatter.cpp"
        # Timing
        "${PROJECT_SOURCE_DIR}/src/utilitybox/timing/timestamp.cpp"

        "${PROJECT_SOURCE_DIR}/src/main.cpp"
    )

# Make Spark Engine core library.
add_executable(spark ${CORE_SOURCE_FILES})

# Enable precompiled headers. #TODO: Check to make sure target_precompile_headers exists in CMake.
target_precompile_headers(spark PRIVATE "${PROJECT_SOURCE_DIR}/include/spark/spark_pch.h")

# Public include files.
set(SPARK_PUBLIC_INCLUDE_FILES "${PROJECT_SOURCE_DIR}/include/spark")
target_include_directories(spark PUBLIC ${SPARK_PUBLIC_INCLUDE_FILES})

# COMPILER
# Set C++ requirements.
target_compile_options(spark PRIVATE -std=c++17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# Set compiler flags.
#target_compile_options(spark PRIVATE -Wall -Wextra -pedantic -Wnon-virtual-dtor -Wno-unused-parameter)
target_compile_options(spark PRIVATE -pthread) # Link with threading library.

# DEPENDENCIES
# Link Glad.
# Glad gives access to both OpenGL extensions and the modern version of the core OpenGL API.
target_link_libraries(spark PUBLIC spark-glad)

# Link GLFW
# GLFW does not depend on OpenGL - OpenGL/Vulkan dependencies get loaded in at runtime.
find_package(OpenGL REQUIRED) # Ensure OpenGL exists on the system.
target_link_libraries(spark PUBLIC glfw)

# ImGui.
target_link_libraries(spark PUBLIC spark-imgui)
