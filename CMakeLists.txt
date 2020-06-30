cmake_minimum_required(VERSION 3.16)
project(DataStructures)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pthread")
set(SOURCE_FILES main.cpp
        Utilities/Memory/segmented_pool_allocator.cpp
        Utilities/Memory/contiguous_pool_allocator.cpp
        Utilities/Memory/memory_formatter.cpp
        Utilities/Logger/logger.cpp
        Utilities/Logger/timestamp.cpp
        Utilities/Logger/log_message.cpp
        Utilities/Logger/logging_system.cpp
        Utilities/Logger/adapter.cpp
        Utilities/Logger/adapter_config.cpp
        Utilities/DataStructures/Array/array.cpp
        Utilities/exception.cpp
        # ECS/Components/component_manager.cpp
        ECS/Components/base_component.cpp
        ECS/Entities/entity.cpp
        ECS/Entities/entity_manager.cpp
        ECS/Systems/base_component_system.tpp
        )

add_executable(DataStructures ${SOURCE_FILES})

