
#include <spark_pch.h>                                   // memset
#include <memory_formatter.h>                            // MemoryFormatter
#include <utilitybox/memory/contiguous_pool_allocator.h> // ContiguousPoolAllocator
#include <utilitybox/tools/assert_dev.h>                 // Asserts

namespace Spark::UtilityBox::Memory {
    class ContiguousPoolAllocator::AllocatorData {
        public:
            // Subscript operator return codes.
            enum class SubscriptOpResult {
                INVALID_INDEX,
                VALID_ACCESS,
            };

            // Holds data about the blocks in one specific page. Creates a linked list with pages.
            struct PageHeader {
                unsigned _numInUse = 0;      // Number of blocks that are in use in this page.
            };

            // Helper object to make linking pages easier.
            struct GenericObject {
                GenericObject *_next = nullptr;
            };

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
            explicit AllocatorData(unsigned blockSize, unsigned numBlocks, bool reallocateOnFull);

            /**
             * Cleans up all pages and returns all memory manager memory back to the OS.
             */
            ~AllocatorData();

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
            _NODISCARD_ void* operator[](unsigned index);

            /**
             * Return a block back to the memory manager.
             * @param blockAddress - Address of the block (given out by RetrieveBlock()) to return.
             */
            void ReturnBlock(void* blockAddress);

            /**
             * Verify that the passed index points to a valid object inside the contiguous memory array. Performs bounds
             * checks.
             * @param index - Index of the object in the memory array.
             * @return INVALID_INDEX - Object at requested index is outside the bounds of the array.
             *         VALID_ACCESS  - Object at requested index was either just marked as allocated or allocated prior.
             */
            SubscriptOpResult CheckSubscriptIndex(unsigned index);

        private:
            /**
             * Construct either an additional page (if a page already exists) or a fresh new page (if a page doesn't
             * already exist).
             */
            void ConstructPage(bool isReallocation);

            /**
             * Construct the free list of a newly allocated page.
             * @param dataBase - Pointer to the base of the page (start of first complete data block).
             */
            void ConstructPageFreeList(void* dataBase, unsigned blockOffset = 0);


            GenericObject* _freeList; // List from which blocks are pulled from to give back to the user upon request.
            PageHeader* _pageHeader;    // Linked list of pages.

            unsigned _blockDataSize;                                            // Size of only the user data block.
            MemoryFormatter _formatter { _blockDataSize, false }; // memory formatter functions.

            unsigned _totalNumBlocks; // Total number of blocks the page can hold.
            bool _reallocateOnFull;   // Flag to reallocate the page with a larger size if there are no more blocks.
            unsigned _totalBlockSize; // Size of an entire block (header, padding, and user data included).
    };

    //------------------------------------------------------------------------------------------------------------------
    // POOL ALLOCATOR DATA
    //------------------------------------------------------------------------------------------------------------------
    // Constructor sets up the bare necessities for the memory manager, but does not initialize data.
    ContiguousPoolAllocator::AllocatorData::AllocatorData(unsigned blockSize, unsigned numBlocks, bool reallocateOnFull) : _freeList(nullptr),
                                                                                                                           _pageHeader(nullptr),
                                                                                                                           _blockDataSize(blockSize),
                                                                                                                           _totalNumBlocks(numBlocks),
                                                                                                                           _reallocateOnFull(reallocateOnFull) {
        _totalBlockSize = _formatter.CalculateMemorySignatureBlockSize() + (_formatter._numPaddingBytes * 2);

        // Allocates a fixed-size page of memory and sets up block lists to use.
        ConstructPage(false);
    }

    // Cleans up all pages and returns all memory manager memory back to the OS.
    ContiguousPoolAllocator::AllocatorData::~AllocatorData() {
        free(_pageHeader);
        _pageHeader = nullptr;
        _freeList = nullptr;
    }

    // Retrieve a free block of memory. Construct additional data stores should there not be any more memory available
    // on the previously allocated page.
    void* ContiguousPoolAllocator::AllocatorData::RetrieveBlock() {
        // Construct new page for blocks if there are no more remaining.
        if (!_freeList) {
            if (_reallocateOnFull) {
                _totalNumBlocks *= 2;
                ConstructPage(true);
            }
            else {
                return nullptr;
            }
        }

        // Get the next free block from the free list.
        void* blockAddress = _freeList;

        // Update the block's header to reflect it being in use.
        ++(_pageHeader->_numInUse);

        // Update free list.
        _freeList = _freeList->_next;

        _formatter.SetBlockDataSignature(blockAddress, _formatter.ALLOCATED);
        return blockAddress;
    }

    void *ContiguousPoolAllocator::AllocatorData::operator[](unsigned int index) {
        return reinterpret_cast<char*>(_pageHeader) + sizeof(PageHeader) + _formatter._numPaddingBytes + (index * _totalBlockSize);
    }

    // Return a block back to the memory manager.
    void ContiguousPoolAllocator::AllocatorData::ReturnBlock(void *blockAddress) {
        // Check padding bytes to ensure memory was not corrupted by data underflow/overflow.
        if (!_formatter.ValidatePaddingBytes(blockAddress)) {
            // todo
        }

        // Clear block data
        _formatter.SetBlockDataSignature(blockAddress, _formatter.UNALLOCATED);

        // Update header to reflect freed block.
        --_pageHeader->_numInUse;

        // Add block to free list.
        GenericObject* previousFreeList = _freeList;
        _freeList = static_cast<GenericObject*>(blockAddress);
        _freeList->_next = previousFreeList;
    }

    // Verify that the passed index points to a valid object inside the contiguous memory array. Performs bounds checks.
    ContiguousPoolAllocator::AllocatorData::SubscriptOpResult ContiguousPoolAllocator::AllocatorData::CheckSubscriptIndex(unsigned index) {
        // Check to make sure block is within the range of the allocated pool.
        if (index >= _totalNumBlocks) {
            return SubscriptOpResult::INVALID_INDEX; // todo more severe error?
        }

        void* blockAddress = operator[](index);

        // Check if the block is on the free list.
        GenericObject* freeList = _freeList;
        GenericObject* freeListPrevious = nullptr;

        while (freeList != nullptr) {

            // Block is on the free list.
            if (reinterpret_cast<void*>(freeList) == blockAddress) {

                // Remove block from the free list to return it.
                // Middle of the free list.
                if (freeListPrevious) {
                    freeListPrevious->_next = freeList->_next;
                    freeList->_next = nullptr;
                }
                // First block on the free list.
                else {
                    GenericObject* block = _freeList;
                    _freeList = _freeList->_next;
                    block->_next = nullptr;
                }

                // Update header and block memory signature to reflect a successful allocated block.
                ++_pageHeader->_numInUse;
                _formatter.SetBlockDataSignature(blockAddress, _formatter.ALLOCATED);

                return ContiguousPoolAllocator::AllocatorData::SubscriptOpResult::VALID_ACCESS;
            }

            // Continue searching.
            freeListPrevious = freeList;
            freeList = freeList->_next;
        }

        // Block is not on the free list and not out of the bounds of the allocated pool, assume points to a valid, already allocated, object.
        return ContiguousPoolAllocator::AllocatorData::SubscriptOpResult::VALID_ACCESS;
    }

    // Construct either an additional page (if a page already exists) or a fresh new page (if a page doesn't already exist).
    void ContiguousPoolAllocator::AllocatorData::ConstructPage(bool isReallocation) {
        //                                         |     SPACE FOR DATA     |
        // ___________________     __________________________________________________________
        // |                 |     |               |             |          |               |
        // |   PAGE HEADER   |  +  |    PADDING    |    NEXT*    |   ....   |    PADDING    |  x blocks per page
        // |_________________|     |_______________|_____________|__________|_______________|
        //
        unsigned pageSize = sizeof(PageHeader) + (_totalBlockSize * _totalNumBlocks);
        // Allocate page.
        void* pageBase = malloc(pageSize);

        if (isReallocation) {
            ASSERT(pageBase != nullptr, "Failed to reallocate memory block of size %i for pool allocator.", pageSize);

            // Offset by the page header pointer to get to the first data block.
            void* dataBase = static_cast<char*>(pageBase) + sizeof(PageHeader);

            // Final block memory signatures.
            for (unsigned i = 0; i < _totalNumBlocks; ++i) {
                _formatter.SetBlockMemorySignatures(static_cast<char *>(dataBase) + (_totalBlockSize * i));
            }

            // Record previous data.
            unsigned numInUse = _pageHeader->_numInUse;

            // Copy over data contents directly.
            unsigned reallocationBytesSize = sizeof(PageHeader) + (_totalBlockSize * (_totalNumBlocks / 2));
            memcpy(pageBase, static_cast<void*>(_pageHeader), reallocationBytesSize);

            // Free old page.
            free(_pageHeader);

            // Reconstruct page header.
            _pageHeader = new (pageBase) PageHeader();
            _pageHeader->_numInUse = numInUse;

            ConstructPageFreeList(dataBase);
        }
        else {
            ASSERT(pageBase != nullptr, "Failed to allocate memory block of size %i for pool allocator.", pageSize);

            // Placement new construct header.
            _pageHeader = new (pageBase) PageHeader();

            // Offset by the page header pointer to get to the first data block.
            void* dataBase = static_cast<char*>(pageBase) + sizeof(PageHeader);

            // Final block memory signatures and link header.
            for (unsigned i = 0; i < _totalNumBlocks; ++i) {
                _formatter.SetBlockMemorySignatures(static_cast<char *>(dataBase) + (_totalBlockSize * i));
            }

            ConstructPageFreeList(dataBase);
        }
    }

    // Construct the free list of a newly allocated page.
    void ContiguousPoolAllocator::AllocatorData::ConstructPageFreeList(void* dataBase, unsigned blockOffset) {
        // Construct free list.
        // Setup first block.
        auto* previousBlock = reinterpret_cast<GenericObject*>(static_cast<char*>(dataBase) + _formatter._numPaddingBytes + (_totalBlockSize * blockOffset));
        _freeList = previousBlock;

        // Set pointers to the rest of the blocks in the list.
        for (++blockOffset; blockOffset < _totalNumBlocks; ++blockOffset) {
            auto* currentBlock = reinterpret_cast<GenericObject*>(reinterpret_cast<char*>(previousBlock) + (_totalBlockSize));
            previousBlock->_next = currentBlock;
            previousBlock = currentBlock;
        }

        previousBlock->_next = nullptr;
    }


    //------------------------------------------------------------------------------------------------------------------
    // POOL ALLOCATOR
    //------------------------------------------------------------------------------------------------------------------
    // Create a fixed-size block memory manager. Provides basic memory debugging information, along with checks for
    // memory corruption. Sets up the bare necessities for the memory manager, but does not initialize data.
    ContiguousPoolAllocator::ContiguousPoolAllocator(unsigned blockSize, unsigned numBlocks, bool reallocateOnFull) : _data(new AllocatorData(blockSize, numBlocks, reallocateOnFull)) {
        // Nothing to do here.
    }

    // Destructor.
    ContiguousPoolAllocator::~ContiguousPoolAllocator() {
        delete _data;
    }

    // Retrieve a free block of memory. Construct additional data stores should there not be any more memory
    void* ContiguousPoolAllocator::RetrieveBlock() {
        return _data->RetrieveBlock();
    }

    // Subscript operator.
    void* ContiguousPoolAllocator::operator[](unsigned index) const {
        // Verify index.
        AllocatorData::SubscriptOpResult subscriptResult = _data->CheckSubscriptIndex(index);

        // Retrieve data.
        switch (subscriptResult) {
            case AllocatorData::SubscriptOpResult::INVALID_INDEX:
                return nullptr;
            case AllocatorData::SubscriptOpResult::VALID_ACCESS:
                return (*_data)[index];
        }

        return nullptr;
    }

    // Return a block back to the memory manager.
    void ContiguousPoolAllocator::ReturnBlock(void *blockAddress) {
        if (blockAddress) {
            _data->ReturnBlock(blockAddress);
        }
    }
}
