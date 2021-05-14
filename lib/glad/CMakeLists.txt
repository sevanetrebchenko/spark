# Add GLAD as a library.
add_library(spark-glad STATIC src/glad.c)

# Set includes for targets using library.
# Any targets that link to the 'glad' target will also need the 'glad' include files.
set(GLAD_INCLUDE_DIRS
        "${PROJECT_SOURCE_DIR}/lib/glad/include")

target_include_directories(spark-glad PUBLIC "${GLAD_INCLUDE_DIRS}")
