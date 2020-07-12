cmake_minimum_required(VERSION 3.16)
project(DataStructures)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pthread")
set(SOURCE_FILES main.cpp
        Utilities/Memory/src/segmented_pool_allocator.cpp
        Utilities/Memory/src/contiguous_pool_allocator.cpp
        Utilities/Memory/src/memory_formatter.cpp
        Utilities/Logger/src/logger.cpp
        Utilities/Logger/src/timestamp.cpp
        Utilities/Logger/src/log_message.cpp
        Utilities/Logger/src/logging_system.cpp
        Utilities/Logger/src/adapter.cpp
        Utilities/Logger/src/adapter_config.cpp
        # ECS/Components/component_manager.cpp
        ECS/Entities/src/entity_manager.cpp
        ECS/Entities/src/entity_manager_data.cpp
        World/world.cpp
        )

add_executable(DataStructures ${SOURCE_FILES})

