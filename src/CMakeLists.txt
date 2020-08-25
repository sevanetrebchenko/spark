# PROJECT FILES
set(CORE_SOURCE_FILES
        # Platform - OpenGL # TODO: Platform specific.
        "${PROJECT_SOURCE_DIR}/src/platform/opengl/renderer/opengl_imgui_layer.cpp"
        "${PROJECT_SOURCE_DIR}/src/platform/opengl/renderer/opengl_rendering_context.cpp"
        "${PROJECT_SOURCE_DIR}/src/platform/opengl/window/opengl_window.cpp"
        # Spark - Core
        "${PROJECT_SOURCE_DIR}/src/spark/core/application.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/core/entry_point.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/core/layer_stack.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/core/layer.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/core/service_locator.cpp"
        # Spark - ECS
        "${PROJECT_SOURCE_DIR}/src/spark/ecs/entities/entity_manager.cpp"
        # Spark - Events
        "${PROJECT_SOURCE_DIR}/src/spark/events/types/event.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/events/types/mouse_events.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/events/types/application_events.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/events/types/key_events.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/events/types/mouse_events.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/events/event_hub.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/events/null_event_hub.cpp"
        # Spark - Graphics
        "${PROJECT_SOURCE_DIR}/src/spark/graphics/renderer/imgui_layer.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/graphics/renderer/renderer_api.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/graphics/renderer/rendering_context.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/graphics/window/window.cpp"
        # Spark - UtilityBox
        # Logger
        "${PROJECT_SOURCE_DIR}/src/spark/utilitybox/logger/adapter.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/utilitybox/logger/adapter_config.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/utilitybox/logger/invalid_format_character_exception.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/utilitybox/logger/logger.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/utilitybox/logger/logging_interface.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/utilitybox/logger/logging_system.cpp"
        # Memory
        "${PROJECT_SOURCE_DIR}/src/spark/utilitybox/memory/contiguous_pool_allocator.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/utilitybox/memory/memory_formatter.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/utilitybox/memory/segmented_pool_allocator.cpp"
        # Timing
        "${PROJECT_SOURCE_DIR}/src/spark/utilitybox/timing/timestamp.cpp"
        )

# Make Spark Engine core library.
add_library(spark ${CORE_SOURCE_FILES})

# Enable precompiled headers. #TODO: Check to make sure target_precompile_headers exists in CMake.
target_precompile_headers(spark PRIVATE "../include/spark/spark_pch.h")

# Public include files.
set(SPARK_PUBLIC_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include")
target_include_directories(spark PUBLIC ${SPARK_PUBLIC_INCLUDE_DIRS})

# COMPILER
# Set C++ requirements.
target_compile_options(spark PUBLIC -std=c++17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# Set compiler flags.
#target_compile_options(spark PRIVATE -Wall -Wextra -pedantic -Wnon-virtual-dtor -Wno-unused-parameter)
target_compile_options(spark PRIVATE -pthread) # Link with threading library.

# DEPENDENCIES
# Link Glad.
# Glad gives access to both OpenGL extensions and the modern version of the core OpenGL API.
target_link_libraries(spark spark-glad)

# Link GLFW
# GLFW does not depend on OpenGL - OpenGL/Vulkan dependencies get loaded in at runtime.
find_package(OpenGL REQUIRED) # Ensure OpenGL exists on the system.
target_link_libraries(spark OpenGL::GL)
target_link_libraries(spark glfw)

# ImGui.
target_link_libraries(spark spark-imgui)

# GLM.
target_link_libraries(spark glm)
