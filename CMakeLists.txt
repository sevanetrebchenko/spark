cmake_minimum_required(VERSION 3.10)
project(Spark)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)


# GLFW3
## use pkg-config to find glfw3 config file that was installed with GLFW binaries
#find_package(PkgConfig REQUIRED)
#pkg_search_module(GLFW REQUIRED glfw3)

# SDL2
# find SDL2 so it's location is known to the compiler
#find_package(SDL2 REQUIRED)
#include_directories(${SDL2_INCLUDE_DIRS})
#message("Including SLD2 libraries found at: ${SDL2_INCLUDE_DIRS}")

set(SOURCE_FILES main.cpp
        src/UtilityBox/Memory/src/segmented_pool_allocator.cpp
        src/UtilityBox/Memory/src/contiguous_pool_allocator.cpp
        src/UtilityBox/Memory/src/memory_formatter.cpp
        src/UtilityBox/Logger/src/logger.cpp
        src/UtilityBox/Logger/src/timestamp.cpp
        src/UtilityBox/Logger/src/log_message.cpp
        src/UtilityBox/Logger/src/logging_system.cpp
        src/UtilityBox/Logger/src/adapter.cpp
        src/UtilityBox/Logger/src/adapter_config.cpp
        # ECS/Components/component_manager.cpp
        src/ECS/Entities/src/entity_manager.cpp
        src/ECS/Entities/src/entity_manager_data.cpp
        src/World/world.cpp
        )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} -pthread)
## link SDL2 libraries to work with the project
#target_link_libraries(${PROJECT_NAME} ${GLFW3_LIBRARIES} -pthread)
#message("Linking GLFW3 libraries with flag: ${GLFW3_LIBRARIES}")
