# CMake version.
cmake_minimum_required(VERSION 3.11...3.16) # 3.11 minimum for finding OpenGL package according to NEW CMake policy.

# Project information.
project(Spark
        VERSION 1.0
        DESCRIPTION "Spark Engine"
        LANGUAGES C CXX)

# Target platform - Linux
# TODO: Support for other systems
if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    message(FATAL_ERROR "Spark Engine is currently only supported by Linux systems.")
endif()

# Set directory for .cmake includes.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# OnUpdate git submodules
include(submodule_setup
        RESULT_VARIABLE SUBMODULE_SETUP_RESULT)
if(SUBMODULE_SETUP_RESULT STREQUAL "NOTFOUND")
    message(FATAL_ERROR "CMake module 'submodule_setup' was not found. Project submodules may be out of date.")
endif()

# Build dependencies.
add_subdirectory(lib)

# Build engine.
add_subdirectory(src)
