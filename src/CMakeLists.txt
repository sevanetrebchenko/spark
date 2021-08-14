# PROJECT FILES
set(CORE_SOURCE_FILES
        # Core / Utility
        "${PROJECT_SOURCE_DIR}/src/spark/core/application.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/core/entry_point.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/core/layer_stack.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/core/layer.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/core/utility.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/utility.cpp"
        # Components
        "${PROJECT_SOURCE_DIR}/src/spark/ecs/components/types/transform.cpp"
        # Systems
        # Entities
        "${PROJECT_SOURCE_DIR}/src/spark/ecs/entities/entity_manager.cpp"
        # Spark - Events
        "${PROJECT_SOURCE_DIR}/src/spark/events/types/mouse_events.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/events/types/application_events.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/events/types/ecs_events.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/events/types/key_events.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/events/types/mouse_events.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/events/event_hub.cpp"
        # LoggingHub
        "${PROJECT_SOURCE_DIR}/src/spark/logger/logger.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/logger/adapter/adapter.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/logger/adapter/adapter_configuration.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/logger/adapter/types/file_adapter.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/logger/adapter/types/standard_output_adapter.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/logger/timestamp.cpp"
        # Memory
        "${PROJECT_SOURCE_DIR}/src/spark/memory/resources/segmented_pool_resource.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/memory/allocators/segmented_pool_allocator.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/memory/allocator.cpp"
        "${PROJECT_SOURCE_DIR}/src/spark/memory/memory_formatter.cpp"
        )

# Make Spark Engine core library.
add_library(spark ${CORE_SOURCE_FILES})

# Enable precompiled headers. #TODO: Check to make sure target_precompile_headers exists in CMake.
target_precompile_headers(spark PRIVATE "../include/spark/utility.h")

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
