
#ifndef DATASTRUCTURES_MEMORY_MANAGER_H
#define DATASTRUCTURES_MEMORY_MANAGER_H

#include "../global_defines.h"
#include <string>

namespace UtilityBox {
    namespace Memory {
        class PoolAllocator final {
            public:
                explicit PoolAllocator(int blockSize);
                void Initialize();

                _NODISCARD_ void* RetrieveBlock();
                void ReturnBlock(void* blockAddress);

            private:
                int _blockSize;

                class PoolAllocatorData;
                PoolAllocatorData* _data
        };
    }
}

#endif // DATASTRUCTURES_MEMORY_MANAGER_H
