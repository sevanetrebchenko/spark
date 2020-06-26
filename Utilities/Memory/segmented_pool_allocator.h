
#ifndef DATASTRUCTURES_SEGMENTED_POOL_ALLOCATOR_H
#define DATASTRUCTURES_SEGMENTED_POOL_ALLOCATOR_H

#include "../global_defines.h"
#include <string>

namespace UtilityBox {
    namespace Memory {
        class SegmentedPoolAllocator final {
            public:
                explicit SegmentedPoolAllocator(int blockSize);
                void Initialize();

                _NODISCARD_ void* RetrieveBlock();
                void ReturnBlock(void* blockAddress);

            private:
                int _blockSize;

                class AllocatorData;
                AllocatorData* _data;
        };
    }
}

#endif // DATASTRUCTURES_SEGMENTED_POOL_ALLOCATOR_H
