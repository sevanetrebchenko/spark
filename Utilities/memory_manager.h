
#ifndef DATASTRUCTURES_MEMORY_MANAGER_H
#define DATASTRUCTURES_MEMORY_MANAGER_H

#include <malloc.h>
#include <string>
#include <array>
#include <iostream>
#include "assert_dev.h"
#include "Logger/logger.h"

#define _NODISCARD_ [[nodiscard]]

namespace UtilityBox {
    namespace MemoryManager {
        _NODISCARD_ void* allocate(const unsigned &, const std::string& = "Not provided.", const void* = nullptr);
        void deallocate(void *location) noexcept;
    }
}

#endif // DATASTRUCTURES_MEMORY_MANAGER_H
