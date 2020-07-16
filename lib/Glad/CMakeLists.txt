
cmake_minimum_required(VERSION 3.16)

project(Glad)
set (SOURCE_FILES src/glad.c)

# Compile the glad source file into a library with the name "Glad".
add_library(glad INTERFACE ${SOURCE_FILES})

# Where to find the header files for this library.
target_include_directories(glad INTERFACE include/)
