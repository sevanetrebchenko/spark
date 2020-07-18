# PROJECT FILES
# TODO: Platform specific.
set(CORE_SOURCE_FILES
        ../main.cpp
        utilitybox/memory/segmented_pool_allocator.cpp
        utilitybox/memory/contiguous_pool_allocator.cpp
        utilitybox/memory/memory_formatter.cpp
        utilitybox/logger/logger.cpp
        utilitybox/logger/timestamp.cpp
        utilitybox/logger/log_message.cpp
        utilitybox/logger/logging_system.cpp
        utilitybox/logger/adapter.cpp
        utilitybox/logger/adapter_config.cpp
        ecs/entities/entity_manager.cpp
        ecs/entities/entity_manager_data.cpp
        World/world.cpp
    )

# Make Spark Engine core library.
add_library(spark STATIC ${CORE_SOURCE_FILES})

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
