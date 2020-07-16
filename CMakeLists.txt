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
        src/ECS/Entities/src/entity_manager.cpp
        src/ECS/Entities/src/entity_manager_data.cpp
        src/World/world.cpp
        )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} -pthread)

# DOXYGEN
# Option to build documentation.
set(BUILD_DOC ON)
message(${CMAKE_CURRENT_BINARY_DIR})
# Only build documentation in Release builds.
if (CMAKE_BUILD_TYPE MATCHES Release)
    # Check to make sure Doxygen is installed.
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        if (BUILD_DOC)
            # Build documentation.
            message("Building Doxygen documentation.")

            # Set input and output files.
            set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
            set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

            # Configure variable values in input file and write to output file.
            configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
            message("Doxygen build started.")

            # note the option ALL which allows to build the docs together with the application
            add_custom_target(Documentation ALL
                    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    COMMENT "Generating API documentation with Doxygen."
                    VERBATIM)
        else (BUILD_DOC)
            message("Doxygen builds turned off.")
        endif(BUILD_DOC)

    else (DOXYGEN_FOUND)
        message("Cannot generate documentation - Doxygen needs to be installed.")
    endif (DOXYGEN_FOUND)

else()
    message("Doxygen only builds in Release.")
endif()

## link SDL2 libraries to work with the project
#target_link_libraries(${PROJECT_NAME} ${GLFW3_LIBRARIES} -pthread)
#message("Linking GLFW3 libraries with flag: ${GLFW3_LIBRARIES}")
