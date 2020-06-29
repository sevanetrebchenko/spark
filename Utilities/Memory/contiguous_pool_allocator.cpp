
#include "contiguous_pool_allocator.h" // ContiguousPoolAllocator
#include "../assert_dev.h"             // Asserts
#include "memory_formatter.h"

namespace UtilityBox::Memory {
    class ContiguousPoolAllocator::AllocatorData {
        public:
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
            explicit AllocatorData(int blockSize, int numBlocks, bool reallocateOnFull);

            /**
             * Two-stage initialization. Allocates a fixed-size page of memory and sets up block lists to use.
             */
            void Initialize();

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
             * Return a block back to the memory manager.
             * @param blockAddress - Address of the block (given out by RetrieveBlock()) to return.
             */
            void ReturnBlock(void* blockAddress);

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
            MemoryFormatter _formatter { _blockDataSize, false }; // Memory formatter functions.

            int _totalNumBlocks;      // Total number of blocks the page can hold.
            bool _reallocateOnFull;   // Flag to reallocate the page with a larger size if there are no more blocks.
            unsigned _totalBlockSize; // Size of an entire block (header, padding, and user data included).
    };

    //------------------------------------------------------------------------------------------------------------------
    // POOL ALLOCATOR DATA
    //------------------------------------------------------------------------------------------------------------------
    // Constructor sets up the bare necessities for the memory manager, but does not initialize data.
    ContiguousPoolAllocator::AllocatorData::AllocatorData(int blockSize, int numBlocks, bool reallocateOnFull) : _freeList(nullptr),
                                                                                                                 _pageHeader(nullptr),
                                                                                                                 _blockDataSize(blockSize),
                                                                                                                 _totalNumBlocks(numBlocks),
                                                                                                                 _reallocateOnFull(reallocateOnFull) {
        _totalBlockSize = _formatter.CalculateMemorySignatureBlockSize() + (_formatter._numPaddingBytes * 2);

        // Defer initialization to second stage.
    }

    // Two-stage initialization. Allocates a fixed-size page of memory and sets up block lists to use.
    void ContiguousPoolAllocator::AllocatorData::Initialize() {
        ConstructPage(false);
    }

    // Cleans up all pages and returns all memory manager memory back to the OS.
    ContiguousPoolAllocator::AllocatorData::~AllocatorData() {
        delete _pageHeader;
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
            for (int i = 0; i < _totalNumBlocks; ++i) {
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
            for (int i = 0; i < _totalNumBlocks; ++i) {
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
    ContiguousPoolAllocator::ContiguousPoolAllocator(int blockSize, int numBlocks, bool reallocateOnFull) : _blockSize(blockSize),
                                                                                                            _numBlocks(numBlocks),
                                                                                                            _reallocateOnFull(reallocateOnFull),
                                                                                                            _data(nullptr) {
        // Defer initialization until second stage.
    }

    // Two-stage initialization. Allocates a fixed-size page of memory and sets up block lists to use.
    void ContiguousPoolAllocator::Initialize() {
        // Construct data if it hasn't been already.
        if (!_data) {
            _data = new AllocatorData(_blockSize, _numBlocks, _reallocateOnFull);
            // todo: try catch?
            _data->Initialize();
        }
    }

    ContiguousPoolAllocator::~ContiguousPoolAllocator() {
        delete _data;
    }

    // Retrieve a free block of memory. Construct additional data stores should there not be any more memory
    void* ContiguousPoolAllocator::RetrieveBlock() {
        return _data->RetrieveBlock();
    }

    // Return a block back to the memory manager.
    void ContiguousPoolAllocator::ReturnBlock(void *blockAddress) {
        if (blockAddress) {
            _data->ReturnBlock(blockAddress);
        }
    }
}
