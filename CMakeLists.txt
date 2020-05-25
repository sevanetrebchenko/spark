cmake_minimum_required(VERSION 3.16)
project(DataStructures)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pthread")
set(SOURCE_FILES main.cpp
        Tree/binary_tree.cpp
        Utilities/memory_manager.cpp
        Utilities/Logger/logger.cpp
        Utilities/Logger/timestamp.cpp
        Utilities/exception.cpp
        Utilities/Logger/log_message.cpp
        )

add_executable(DataStructures ${SOURCE_FILES})

