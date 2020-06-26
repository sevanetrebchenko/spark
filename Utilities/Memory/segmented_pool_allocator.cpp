
#include "segmented_pool_allocator.h" // SegmentedPoolAllocator
#include "../assert_dev.h"            // Asserts

namespace UtilityBox::Memory {
    class SegmentedPoolAllocator::AllocatorData {
        public:
            // Holds data about the blocks in one specific page. Creates a linked list with pages.
            struct PageHeader {
                PageHeader* _next = nullptr; // Next page pointer.
                unsigned _numInUse = 0;      // Number of blocks that are in use in this page.
            };

            // Helper object to make linking pages easier.
            struct GenericObject {
                GenericObject *_next = nullptr;
            };

            /**
             * Constructor sets up the bare necessities for the memory manager, but does not initialize data.
             * @param blockSize - Size of block this memory manager manages.
             */
            explicit AllocatorData(int blockSize);

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
            void ConstructPage();

            /**
             * Set debug memory signatures for a block.
             * @param blockAddress - Block to set memory signature.
             */
            void SetBlockMemorySignatures(void* blockAddress) const;

            /**
             * Link the page header to a block within the page.
             * @param blockAddress - Block to set the header of.
             * @param pageHeader   - Page header.
             */
            void SetBlockHeader(void* blockAddress, void* pageHeader) const;

            /**
             * Construct the free list of a newly allocated page.
             * @param dataBase - Pointer to the base of the page (start of first complete data block).
             */
            void ConstructPageFreeList(void* dataBase);

            /**
             * Validate padding bytes on either side of the user data block to ensure no data corruption happened.
             * @param blockAddress - Pointer (provided to RetrieveBlock()) of block to check the padding bytes of.
             * @return On success: true (bytes are intact - no data corruption).
             *         On failure: false (bytes are modified - data corruption most likely occurred).
             */
            bool ValidatePaddingBytes(void* blockAddress) const;

            /**
             * Returns the memory held by empty/unused pages back to the OS.
             */
            void FreeEmptyPages();


            // Memory signatures.
            const unsigned char UNALLOCATED = 0xCC;
            const unsigned char ALLOCATED = 0xAA;
            const unsigned char PADDING = 0xFF;

            GenericObject* _freeList; // List from which blocks are pulled from to give back to the user upon request.
            PageHeader* _pageList;    // Linked list of pages.

            const unsigned _blocksPerPage = 10;  // Number of blocks per page.
            const unsigned _numPaddingBytes = 4; // Number of padding bytes on either side of the user data block.
            unsigned _numPages;                  // Current number of pages in use.
            unsigned _blockDataSize;             // Size of only the user data block.
            unsigned _totalBlockSize;            // Size of an entire block (header, padding, and user data included).
    };

    //------------------------------------------------------------------------------------------------------------------
    // POOL ALLOCATOR DATA
    //------------------------------------------------------------------------------------------------------------------
    // Constructor sets up the bare necessities for the memory manager, but does not initialize data.
    SegmentedPoolAllocator::AllocatorData::AllocatorData(int blockSize) : _freeList(nullptr),
                                                                          _pageList(nullptr),
                                                                          _numPages(0),
                                                                          _blockDataSize(blockSize) {
        // If size of data is larger than pointer, only use the size of the data.
        // Pointer (used for the free list) will be overwritten when used.
        if (blockSize >= sizeof(void*)) {
            _totalBlockSize = sizeof(void*) + (_numPaddingBytes * 2) + blockSize;
        }
        // A block needs to at least be able to hold a pointer (if the data size is smaller than than of a pointer).
        else {
            _totalBlockSize = 2 * sizeof(void*) + (_numPaddingBytes * 2);
        }

        // Defer initialization to second stage.
    }

    // Two-stage initialization. Allocates a fixed-size page of memory and sets up block lists to use.
    void SegmentedPoolAllocator::AllocatorData::Initialize() {
        ConstructPage();
    }

    // Cleans up all pages and returns all memory manager memory back to the OS.
    SegmentedPoolAllocator::AllocatorData::~AllocatorData() {
        // Todo
    }

    // Retrieve a free block of memory. Construct additional data stores should there not be any more memory available
    // on the previously allocated page.
    void* SegmentedPoolAllocator::AllocatorData::RetrieveBlock() {
        // Construct new page for blocks if there are no more remaining.
        if (!_freeList) {
            ConstructPage();
        }

        // Get the next free block from the free list.
        void* blockAddress = _freeList;

        // Update the block's header to reflect it being in use.
        auto* blockHeader = reinterpret_cast<PageHeader*>(reinterpret_cast<GenericObject*>(static_cast<char*>(blockAddress) - sizeof(void*) - _numPaddingBytes)->_next);
        ++blockHeader->_numInUse;

        // Update free list.
        _freeList = _freeList->_next;

        memset(blockAddress, ALLOCATED, _blockDataSize);
        return blockAddress;
    }

    // Return a block back to the memory manager.
    void SegmentedPoolAllocator::AllocatorData::ReturnBlock(void *blockAddress) {
        // Check padding bytes to ensure memory was not corrupted by data underflow/overflow.
        if (!ValidatePaddingBytes(blockAddress)) {
            // todo
        }

        // Clear block data
        memset(blockAddress, UNALLOCATED, _blockDataSize);

        // Offset the pointer to gain access to the header
        auto* blockHeader = reinterpret_cast<PageHeader*>(reinterpret_cast<GenericObject*>(static_cast<char*>(blockAddress) - sizeof(void*) - _numPaddingBytes)->_next);

        // Update header to reflect freed block.
        --blockHeader->_numInUse;

        // Add block to free list.
        GenericObject* previousFreeList = _freeList;
        _freeList = static_cast<GenericObject*>(blockAddress);
        _freeList->_next = previousFreeList;
    }

    // Construct either an additional page (if a page already exists) or a fresh new page (if a page doesn't already exist).
    void SegmentedPoolAllocator::AllocatorData::ConstructPage() {
        //                                                        |     SPACE FOR DATA     |
        // ___________________     _________________________________________________________________________
        // |                 |     |              |               |             |          |               |
        // |   PAGE HEADER   |  +  |    HEADER*   |    PADDING    |    NEXT*    |   ....   |    PADDING    |  x blocks per page
        // |_________________|     |______________|_______________|_____________|__________|_______________|
        //

        unsigned pageSize = sizeof(PageHeader) + (_totalBlockSize * _blocksPerPage);

        // Allocate page.
        void* pageBase = malloc(pageSize);
        ASSERT(pageBase != nullptr, "Failed to allocate memory for pool allocator.");

        PageHeader* currentPageHeader;

        // Page list is null (this is the first page).
        if (!_pageList) {
            // Placement new.
            _pageList = new (pageBase) PageHeader();

            currentPageHeader = _pageList;
        }
        // Page list already exists (this is not the first page).
        else {
            // Get to the last page.
            PageHeader* pageList = _pageList;
            while (pageList->_next) {
                pageList = pageList->_next;
            }

            // Set the next page link.
            pageList->_next = static_cast<PageHeader*>(pageBase);

            currentPageHeader = pageList->_next;
        }

        // Offset by the page header pointer to get to the first data block.
        void* dataBase = static_cast<char*>(pageBase) + sizeof(PageHeader);

        // Final block memory signatures and link header.
        for (int i = 0; i < _blocksPerPage; ++i) {
            SetBlockMemorySignatures(static_cast<char *>(dataBase) + (_totalBlockSize * i));
            SetBlockHeader(static_cast<char *>(dataBase) + (_totalBlockSize * i), currentPageHeader);
        }

        ConstructPageFreeList(dataBase);
        ++_numPages;
    }

    // Set debug memory signatures for a block.
    void SegmentedPoolAllocator::AllocatorData::SetBlockMemorySignatures(void* blockAddress) const {
        unsigned offset = sizeof(void*);

        // Set padding memory signature.
        memset(static_cast<char*>(blockAddress) + offset, PADDING, _numPaddingBytes);
        offset += _numPaddingBytes;

        // Set unallocated memory signature.
        memset(static_cast<char*>(blockAddress) + offset, UNALLOCATED, _blockDataSize);
        offset += _blockDataSize;

        // Set padding memory signature.
        memset(static_cast<char*>(blockAddress) + offset, PADDING, _numPaddingBytes);
        offset += _numPaddingBytes;
    }

    // Link the page header to a block within the page.
    void SegmentedPoolAllocator::AllocatorData::SetBlockHeader(void* blockAddress, void *pageHeader) const {
        // Set block header.
        static_cast<GenericObject*>(blockAddress)->_next = static_cast<GenericObject*>(pageHeader);
    }

    // Construct the free list of a newly allocated page.
    void SegmentedPoolAllocator::AllocatorData::ConstructPageFreeList(void *dataBase) {
        // Construct free list.
        // Setup first block.
        auto* previousBlock = reinterpret_cast<GenericObject*>(static_cast<char*>(dataBase) + _numPaddingBytes + sizeof(void*));
        _freeList = previousBlock;

        // Set pointers to the rest of the blocks in the list.
        for (int i = 1; i < _blocksPerPage; ++i) {
            auto* currentBlock = reinterpret_cast<GenericObject*>(reinterpret_cast<char*>(previousBlock) + (_totalBlockSize));
            previousBlock->_next = currentBlock;
            previousBlock = currentBlock;
        }

        previousBlock->_next = nullptr;
    }

    // Validate padding bytes on either side of the user data block to ensure no data corruption happened.
    bool SegmentedPoolAllocator::AllocatorData::ValidatePaddingBytes(void *blockAddress) const {
        // Check padding bytes before data.
        char* preBlockPaddingBytes = static_cast<char*>(blockAddress) - _numPaddingBytes;
        for (int i = 0; i < _numPaddingBytes; ++i) {
            if (preBlockPaddingBytes[i] != PADDING) {
                return false;
            }
        }

        // Check padding bytes after data.
        char* postBlockPaddingBytes = static_cast<char*>(blockAddress) + _blockDataSize;
        for (int i = 0; i < _numPaddingBytes; ++i) {
            if (postBlockPaddingBytes[i] != PADDING) {
                return false;
            }
        }

        return true;
    }

    // Returns the memory held by empty/unused pages back to the OS.
    void SegmentedPoolAllocator::AllocatorData::FreeEmptyPages() {
        PageHeader* pageList = _pageList;
        PageHeader* previousPage = nullptr;
        while (pageList) {
            if (pageList->_numInUse == 0) {
                if (previousPage) {
                    previousPage->_next = pageList->_next;
                }
            }

            previousPage = pageList;
            pageList = pageList->_next;

            free(previousPage);
        }
    }

    //------------------------------------------------------------------------------------------------------------------
    // POOL ALLOCATOR
    //------------------------------------------------------------------------------------------------------------------
    // Create a fixed-size block memory manager. Provides basic memory debugging information, along with checks for
    // memory corruption. Sets up the bare necessities for the memory manager, but does not initialize data.
    SegmentedPoolAllocator::SegmentedPoolAllocator(int blockSize) : _blockSize(blockSize), _data(nullptr) {
        // Defer initialization until second stage.
    }

    // Two-stage initialization. Allocates a fixed-size page of memory and sets up block lists to use.
    void SegmentedPoolAllocator::Initialize() {
        // Construct data if it hasn't been already.
        if (!_data) {
            _data = new AllocatorData(_blockSize);
            // todo: try catch?
            _data->Initialize();
        }
    }

    // Retrieve a free block of memory. Construct additional data stores should there not be any more memory
    void* SegmentedPoolAllocator::RetrieveBlock() {
        return _data->RetrieveBlock();
    }

    // Return a block back to the memory manager.
    void SegmentedPoolAllocator::ReturnBlock(void *blockAddress) {
        _data->ReturnBlock(blockAddress);
    }
}
