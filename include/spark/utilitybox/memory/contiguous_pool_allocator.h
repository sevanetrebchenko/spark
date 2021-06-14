
#ifndef SPARK_CONTIGUOUS_POOL_ALLOCATOR_H
#define SPARK_CONTIGUOUS_POOL_ALLOCATOR_H

#include <spark/core/core.h>
#include <spark/utilitybox/memory/allocator_interface.h>

namespace Spark {
    namespace UtilityBox {
        namespace Memory {

            class ContiguousPoolAllocator : public IAllocator {
                public:
                    ContiguousPoolAllocator(unsigned blockSize, unsigned numBlocks, bool reallocateOnFull);
                    ~ContiguousPoolAllocator();

                    _NODISCARD_ void* RetrieveBlock() override;
                    _NODISCARD_ void* operator[](unsigned index) const;
                    void ReturnBlock(void* blockAddress) override;

                private:
                    // Storage for ContiguousPoolAllocator data, back-end functionality, and helper functions.
                    class AllocatorData;
                    AllocatorData* _data = nullptr;
            };

        }
    }
}

#endif // SPARK_CONTIGUOUS_POOL_ALLOCATOR_H
