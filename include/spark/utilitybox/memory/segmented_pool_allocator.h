
#ifndef SPARK_SEGMENTED_POOL_ALLOCATOR_H
#define SPARK_SEGMENTED_POOL_ALLOCATOR_H

#include <spark/core/rename.h>
#include <spark/utilitybox/memory/allocator.h> // IAllocator

namespace Spark {
    namespace UtilityBox {
        namespace Memory {

            class SegmentedPoolAllocator {
                public:
                    explicit SegmentedPoolAllocator(unsigned blockSize);
                    ~SegmentedPoolAllocator();

                    NODISCARD void* RetrieveBlock();
                    void ReturnBlock(void* blockAddress);

                private:
                    // Holds data about the blocks in one specific page. Creates a linked list with pages.
                    struct PageHeader {
                        PageHeader* _next = nullptr; // Next page pointer.
                        unsigned _numInUse = 0;      // Number of blocks that are in use in this page.
                    };

                    // Helper object to make linking pages easier.
                    struct GenericObject {
                        GenericObject *_next = nullptr;
                    };

                    void ConstructPage();
                    void SetBlockHeader(void* blockAddress, void* pageHeader) const;
                    void ConstructPageFreeList(void* dataBase);
                    void FreeEmptyPages();

                    GenericObject* _freeList; // List from which blocks are pulled from to give back to the user upon request.
                    PageHeader* _pageList;    // Linked list of pages.

                    unsigned _blockDataSize;                                           // Size of only the user data block.
                    // MemoryFormatter _formatter { _blockDataSize, true }; // memory formatter functions.

                    const unsigned _blocksPerPage = 10;  // Number of blocks per page.
                    const unsigned _numPaddingBytes = 4; // Number of padding bytes on either side of the user data block.
                    unsigned _numPages;                  // Current number of pages in use.
                    unsigned _totalBlockSize;            // Size of an entire block (header, padding, and user data included).
            };

        }
    }
}

#endif // SPARK_SEGMENTED_POOL_ALLOCATOR_H
