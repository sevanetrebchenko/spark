cmake_minimum_required(VERSION 3.16)
project(DataStructures)

set(CMAKE_CXX_STANDARD 17)

add_executable(DataStructures main.cpp Tree/binary_tree.cpp Tree/binary_tree.h Utilities/memory_manager.cpp Utilities/memory_manager.h Utilities/logger.cpp Utilities/logger.h Utilities/assert_dev.h)
