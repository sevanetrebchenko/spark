cmake_minimum_required(VERSION 3.16)
project(DataStructures)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(DataStructures main.cpp Tree/binary_tree.cpp Tree/binary_tree.h Utilities/memory_manager.cpp Utilities/memory_manager.h Utilities/Logger/logger.cpp Utilities/Logger/logger.h Utilities/assert_dev.h Utilities/Logger/timestamp.cpp Utilities/Logger/timestamp.h Utilities/Logger/exception.cpp Utilities/Logger/exception.h)

