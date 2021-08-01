
#ifndef SPARK_ALLOCATOR_H
#define SPARK_ALLOCATOR_H

#include <spark/core/rename.h>
#include <spark/utilitybox/memory/memory_resource.h>

namespace Spark {
    namespace UtilityBox {
        namespace Memory {

            // C++17 compliant allocator.
            template <typename T, typename MemoryResource>
            class IAllocator {
                public:
                    typedef T value_type;

                    IAllocator();
                    virtual ~IAllocator();

                    template <typename U, typename Other>
                    NODISCARD IAllocator<T, MemoryResource>& operator=(const IAllocator<U, Other>& other) {
                        return *this;
                    }

//                    NODISCARD virtual constexpr void* allocate(std::size_t numBytes) = 0;
//                    NODISCARD virtual constexpr void* allocate(std::size_t numBytes, const void* hint) = 0;
//                    virtual constexpr void deallocate(void* address, std::size_t numBytes) = 0;
//
//                    NODISCARD virtual bool operator==(const IAllocator& other) const = 0;
//                    NODISCARD virtual bool operator!=(const IAllocator& other) const = 0;

                private:
                    MemoryResource _memoryResource;
            };

        }
    }
}

#endif //SPARK_ALLOCATOR_H
