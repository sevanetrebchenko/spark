cmake_minimum_required(VERSION 3.10)
project(DataStructures)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# GLFW3
# use pkg-config to find glfw3 config file that was installed with GLFW binaries
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

# SDL2
# find SDL2 so it's location is known to the compiler
#find_package(SDL2 REQUIRED)
#include_directories(${SDL2_INCLUDE_DIRS})
#message("Including SLD2 libraries found at: ${SDL2_INCLUDE_DIRS}")

set(SOURCE_FILES main.cpp
        UtilityBox/Memory/src/segmented_pool_allocator.cpp
        UtilityBox/Memory/src/contiguous_pool_allocator.cpp
        UtilityBox/Memory/src/memory_formatter.cpp
        UtilityBox/Logger/src/logger.cpp
        UtilityBox/Logger/src/timestamp.cpp
        UtilityBox/Logger/src/log_message.cpp
        UtilityBox/Logger/src/logging_system.cpp
        UtilityBox/Logger/src/adapter.cpp
        UtilityBox/Logger/src/adapter_config.cpp
        # ECS/Components/component_manager.cpp
        ECS/Entities/src/entity_manager.cpp
        ECS/Entities/src/entity_manager_data.cpp
        World/world.cpp
        )

add_executable(DataStructures ${SOURCE_FILES})
# link SDL2 libraries to work with the project
target_link_libraries(DataStructures ${GLFW3_LIBRARIES} -pthread)
message("Linking GLFW3 libraries with flag: ${GLFW3_LIBRARIES}")
