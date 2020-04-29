cmake_minimum_required(VERSION 3.16)
project(DataStructures)

set(CMAKE_CXX_STANDARD 17)

add_executable(DataStructures main.cpp Tree/binary_tree.cpp Tree/binary_tree.h MemoryManager/memory_manager.cpp MemoryManager/memory_manager.h)