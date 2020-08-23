# PROJECT FILES
set(CORE_SOURCE_FILES
        # Platform - OpenGL # TODO: Platform specific.
        "${PROJECT_SOURCE_DIR}/src/platform/opengl/renderer/opengl_imgui_layer.cpp"
        "${PROJECT_SOURCE_DIR}/src/platform/opengl/renderer/opengl_renderer_api.cpp"
        "${PROJECT_SOURCE_DIR}/src/platform/opengl/renderer/opengl_rendering_context.cpp"
        "${PROJECT_SOURCE_DIR}/src/platform/opengl/window/opengl_window.cpp"
        # Spark - Core
        "${PROJECT_SOURCE_DIR}/src/spark/src/core/application.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/src/core/entry_point.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/src/core/layer_stack.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/src/core/layer.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/src/core/service_locator.cpp"
        # Spark - ECS
        "${PROJECT_SOURCE_DIR}/src/spark/src/ecs/entities/entity_manager.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/src/ecs/entities/entity_manager_data.cpp"
        # Spark - Events
        "${PROJECT_SOURCE_DIR}/src/spark/src/events/types/event.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/src/events/types/mouse_events.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/src/events/types/application_events.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/src/events/types/key_events.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/src/events/types/mouse_events.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/src/events/event_hub.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/src/events/null_event_hub.cpp"
        # Spark - Graphics
        "${PROJECT_SOURCE_DIR}/src/spark/src/graphics/renderer/imgui_layer.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/src/graphics/renderer/renderer_api.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/src/graphics/renderer/rendering_context.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/src/graphics/window/window.cpp"
        # Spark - UtilityBox
        # Logger
        "${PROJECT_SOURCE_DIR}/src/spark/src/utilitybox/logger/adapter.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/src/utilitybox/logger/adapter_config.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/src/utilitybox/logger/invalid_format_character_exception.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/src/utilitybox/logger/logger.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/src/utilitybox/logger/logging_interface.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/src/utilitybox/logger/logging_system.cpp"
        # Memory
        "${PROJECT_SOURCE_DIR}/src/spark/src/utilitybox/memory/contiguous_pool_allocator.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/src/utilitybox/memory/memory_formatter.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/src/utilitybox/memory/segmented_pool_allocator.cpp"
        # Timing
        "${PROJECT_SOURCE_DIR}/src/spark/src/utilitybox/timing/timestamp.cpp"
        )

# Make Spark Engine core library.
add_executable(spark ${CORE_SOURCE_FILES})

# Enable precompiled headers. #TODO: Check to make sure target_precompile_headers exists in CMake.
target_precompile_headers(spark PRIVATE "${PROJECT_SOURCE_DIR}/src/spark/include/spark_pch.h")

# Public include files.
set(SPARK_PUBLIC_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include")
target_include_directories(spark PUBLIC ${SPARK_PUBLIC_INCLUDE_DIRS})

# Private include files
set(SPARK_PRIVATE_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src/spark/include")
target_include_directories(spark PRIVATE ${SPARK_PRIVATE_INCLUDE_DIRS})

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
