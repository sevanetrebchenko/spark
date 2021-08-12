#ifndef SPARK_MEMORY_RESOURCE_H
#define SPARK_MEMORY_RESOURCE_H

#include <spark/core/rename.h>

namespace Spark {
    namespace UtilityBox {
        namespace Memory {

            class IMemoryResource1 {
                public:
                    IMemoryResource1();
                    virtual ~IMemoryResource1();

//                    NODISCARD virtual constexpr void* allocate(std::size_t numBytes, const void* hint) = 0;
//                    virtual constexpr void deallocate(void* address, std::size_t numBytes) = 0;
            };

            class IMemoryResource2 {
                public:
                    IMemoryResource2();
                    virtual ~IMemoryResource2();

//                    NODISCARD virtual constexpr void* allocate(std::size_t numBytes, const void* hint) = 0;
//                    virtual constexpr void deallocate(void* address, std::size_t numBytes) = 0;
            };

        }
    }
}

#endif //SPARK_MEMORY_RESOURCE_H
