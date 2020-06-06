
#ifndef DATASTRUCTURES_MEMORY_MANAGER_H
#define DATASTRUCTURES_MEMORY_MANAGER_H

#include "global_defines.h"
#include <malloc.h>
#include <string>
#include <array>
#include <iostream>
#include "assert_dev.h"
#include "Logger/logger.h"

namespace UtilityBox {
    namespace MemoryManager {
        _NODISCARD_ void* allocate(const unsigned &, const std::string& = "Not provided.", const void* = nullptr);
        void deallocate(void *location) noexcept;
    }
}

#endif // DATASTRUCTURES_MEMORY_MANAGER_H
