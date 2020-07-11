
#ifndef DATASTRUCTURES_SEGMENTED_POOL_ALLOCATOR_H
#define DATASTRUCTURES_SEGMENTED_POOL_ALLOCATOR_H

#include "../Tools/global_defines.h" // _NODISCARD_

namespace UtilityBox::Memory {
        class SegmentedPoolAllocator final {
            public:
                /**
                 * Create a fixed-size block memory manager. Provides basic memory debugging information, along with
                 * checks for memory corruption. Sets up the bare necessities for the memory manager, but does not
                 * initialize data.
                 * Note: Initialize() must be called in order for the memory manager to work properly.
                 */
                explicit SegmentedPoolAllocator(unsigned blockSize);

                /**
                 * Cleans up all pages and returns all memory manager memory back to the OS.
                 */
                ~SegmentedPoolAllocator();

                /**
                 * Retrieve a free block of memory. Construct additional data stores should there not be any more memory
                 * available on the previously allocated page.
                 * @return - Pointer to the allocated block of memory.
                 */
                _NODISCARD_ void* RetrieveBlock();

                /**
                 * Return a block back to the memory manager.
                 * Note: This operation does not call destructors for the block - ensure that the data that the block
                 * held has been properly cleaned up to avoid memory leaks.
                 * @param blockAddress - Address of the block (given out by RetrieveBlock()) to return.
                 */
                void ReturnBlock(void* blockAddress);

            private:
                // Storage for SegmentedPoolAllocator data, back-end functionality, and helper functions.
                class AllocatorData;
                AllocatorData* _data = nullptr;
        };
    }

#endif // DATASTRUCTURES_SEGMENTED_POOL_ALLOCATOR_H
