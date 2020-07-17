
cmake_minimum_required(VERSION 3.16)

project(glad)
add_library(glad INTERFACE)
# Compile the glad source file into a library with the name "glad".
target_sources(glad INTERFACE src/glad.c)

# Where to find the header files for this library.
target_include_directories(glad INTERFACE include/)
