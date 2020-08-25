
# Project information.
project(Sandbox
        VERSION 1.0
        DESCRIPTION "Spark Engine - Sandbox"
        LANGUAGES C CXX)

# PROJECT FILES
set(CORE_SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/src/sandbox.cpp"
        "${PROJECT_SOURCE_DIR}/src/sandbox_layer.cpp"
        )

# Make Sandbox.
add_executable(sandbox ${CORE_SOURCE_FILES})

# Link to Spark engine.
target_link_libraries(sandbox PRIVATE spark)

# Set Sandbox public include directories.
set(SANDBOX_PUBLIC_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include")
target_include_directories(sandbox PUBLIC ${SANDBOX_PUBLIC_INCLUDE_DIRS})

# Spark and platform-specific include directories.
set(SANDBOX_PRIVATE_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/include")
target_include_directories(sandbox PRIVATE ${SANDBOX_PRIVATE_INCLUDE_DIRS})

