
#ifndef SPARK_CONTIGUOUS_POOL_ALLOCATOR_H
#define SPARK_CONTIGUOUS_POOL_ALLOCATOR_H

#include <spark/utilitybox/tools/global_defines.h> // _NODISCARD_

namespace Spark {
    namespace UtilityBox {
        namespace Memory {

            class ContiguousPoolAllocator final {
                public:
                    /**
                    * Create a fixed-size block memory manager. Provides basic memory debugging information, along with
                    * checks for memory corruption. Sets up the bare necessities for the memory manager, but does not
                    * initialize data.
                    * Note: Initialize() must be called in order for the memory manager to work properly.
                    * @param blockSize        - Size of block memory manager holds.
                    * @param numBlocks        - Starting number of blocks to allocate.
                    * @param reallocateOnFull - True:  reallocates page if more than capacity blocks are requested (invalidates any pointers).
                    *                           False: returns nullptr when more than capacity blocks are requested (leaves page untouched).
                    */
                    explicit ContiguousPoolAllocator(unsigned blockSize, unsigned numBlocks, bool reallocateOnFull);

                    /**
                    * Cleans up all pages and returns all memory manager memory back to the OS.
                    */
                    ~ContiguousPoolAllocator();

                    /**
                    * Retrieve a free block of memory. Construct additional data stores should there not be any more memory
                    * available on the previously allocated page.
                    * @return - Pointer to the allocated block of memory.
                    */
                    _NODISCARD_ void* RetrieveBlock();

                    /**
                    * Subscript operator.
                    * @param index - Does index bound checking and returns the object in the array at the given index.
                    * @return On success: Pointer to the location at the provided index.
                    *         On failure: nullptr.
                    */
                    _NODISCARD_ void* operator[](unsigned index) const;

                    /**
                    * Return a block back to the memory manager.
                    * Note: This operation does not call destructors for the block - ensure that the data that the block
                    * held has been properly cleaned up to avoid memory leaks.
                    * @param blockAddress - Address of the block (given out by RetrieveBlock()) to return.
                    */
                    void ReturnBlock(void* blockAddress);

                private:
                    // Storage for ContiguousPoolAllocator data, back-end functionality, and helper functions.
                    class AllocatorData;
                    AllocatorData* _data = nullptr;
            };

        }
    }
}

#endif // SPARK_CONTIGUOUS_POOL_ALLOCATOR_H
