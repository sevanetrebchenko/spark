
#ifndef DATASTRUCTURES_MEMORY_MANAGER_H
#define DATASTRUCTURES_MEMORY_MANAGER_H

#include <cstddef>
#include <malloc.h>

#define _NODISCARD [[nodiscard]]

struct MemoryManagerConfiguration {
    // Memory signatures for debug purposes
    static const unsigned char UNALLOCATED = 0xFF;
    static const unsigned char ALLOCATED = 0xCC;
    static const unsigned char PADDING = 0xBB;
    static const unsigned char ALIGNMENT = 0xAA;

    enum AllocationPolicy {
        MM_FIRST_FIT,
        MM_NEXT_FIT,
        MM_BEST_FIT,
        MM_NOT_APPLICABLE
    };

    enum Allocation

    MemoryManagerConfiguration();
    MemoryManagerConfiguration(MemoryManagerConfiguration const &&rvalue);

    // DATA
    AllocationPolicy policy;
    unsigned alignment = sizeof(void*);
};

template<typename T, typename Configuration = MemoryManagerConfiguration>
class MemoryManager {
    public:
        typedef T                 value_type;
        typedef value_type*       pointer;
        typedef value_type const* const_pointer;
        typedef value_type&       reference;
        typedef value_type const& const_reference;
        typedef std::size_t       size_type;

        MemoryManager();
        ~MemoryManager();

        /**
         * Allocate formatted memory based on the provided MemoryManagerConfiguration
         * @param bytes Number of bytes to allocate memory for.
         * @param hint Hint for the allocation policy to use to allocate to reduce memory fragmentation
         * @return
         */
        _NODISCARD pointer allocate(size_type bytes, const_pointer hint = nullptr);

        /**
         *
         * @param ptr
         */
        void deallocate(const_pointer ptr) noexcept;

    private:
        MemoryManagerConfiguration _config;
};

#endif //DATASTRUCTURES_MEMORY_MANAGER_H
