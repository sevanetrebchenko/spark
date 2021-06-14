
#include <spark/core/core.h>
#include <spark/utilitybox/memory/memory_formatter.h>         // MemoryFormatter
#include <spark/utilitybox/memory/segmented_pool_allocator.h> // SegmentedPoolAllocator

namespace Spark::UtilityBox::Memory {

    // Create a fixed-size block memory manager. Provides basic memory debugging information, along with checks for
    // memory corruption. Sets up the bare necessities for the memory manager, but does not initialize data.
    SegmentedPoolAllocator::SegmentedPoolAllocator(unsigned blockSize) : _freeList(nullptr),
                                                                         _pageList(nullptr),
                                                                         _blockDataSize(blockSize),
                                                                         _numPages(0)
                                                                         {
        // Header pointer + padding + user block + padding
        _totalBlockSize = sizeof(void*) + _formatter.CalculateMemorySignatureBlockSize() + (_formatter._numPaddingBytes * 2);

        // Allocates a fixed-size page of memory and sets up block lists to use.
        ConstructPage();
    }

    // Cleans up all pages and returns all memory manager memory back to the OS.
    SegmentedPoolAllocator::~SegmentedPoolAllocator() {
        PageHeader* currentHeader;

        // Free all pages.
        while (_pageList != nullptr) {
            currentHeader = _pageList;
            _pageList = _pageList->_next;
            free(currentHeader);
        }

        _pageList = nullptr;
        _freeList = nullptr;
    }

    // Retrieve a free block of memory. Construct additional data stores should there not be any more memory available
    // on the previously allocated page.
    void* SegmentedPoolAllocator::RetrieveBlock() {
        // Construct new page for blocks if there are no more remaining.
        if (!_freeList) {
            ConstructPage();
        }

        // Get the next free block from the free list.
        void* blockAddress = _freeList;

        // OnUpdate the block's header to reflect it being in use.
        auto* blockHeader = reinterpret_cast<PageHeader*>(reinterpret_cast<GenericObject*>(static_cast<char*>(blockAddress) - sizeof(void*) - _numPaddingBytes)->_next);
        ++blockHeader->_numInUse;

        // OnUpdate free list.
        _freeList = _freeList->_next;

        _formatter.SetBlockDataSignature(blockAddress, _formatter.ALLOCATED);
        return blockAddress;
    }

    // Return a block back to the memory manager.
    void SegmentedPoolAllocator::ReturnBlock(void *blockAddress) {
        // Check padding bytes to ensure memory was not corrupted by data underflow/overflow.
        if (!_formatter.ValidatePaddingBytes(blockAddress)) {
            throw std::runtime_error("Failed to validate padding bytes of a returned block - memory corruption likely.");
        }

        // Clear block data
        _formatter.SetBlockDataSignature(blockAddress, _formatter.UNALLOCATED);

        // Offset the pointer to gain access to the header
        auto* blockHeader = reinterpret_cast<PageHeader*>(reinterpret_cast<GenericObject*>(static_cast<char*>(blockAddress) - sizeof(void*) - _numPaddingBytes)->_next);

        // OnUpdate header to reflect freed block.
        --blockHeader->_numInUse;

        // Add block to free list.
        GenericObject* previousFreeList = _freeList;
        _freeList = static_cast<GenericObject*>(blockAddress);
        _freeList->_next = previousFreeList;
    }

    // Construct either an additional page (if a page already exists) or a fresh new page (if a page doesn't already exist).
    void SegmentedPoolAllocator::ConstructPage() {
        //                                                        |     SPACE FOR DATA     |
        // ___________________     _________________________________________________________________________
        // |                 |     |              |               |             |          |               |
        // |   PAGE HEADER   |  +  |    HEADER*   |    PADDING    |    NEXT*    |   ....   |    PADDING    |  x blocks per page
        // |_________________|     |______________|_______________|_____________|__________|_______________|
        //

        unsigned pageSize = sizeof(PageHeader) + (_totalBlockSize * _blocksPerPage);

        // Allocate page.
        void* pageBase = malloc(pageSize);

        if (!pageBase) {
            throw std::bad_alloc();
        }

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
        for (unsigned i = 0; i < _blocksPerPage; ++i) {
            _formatter.SetBlockMemorySignatures(static_cast<char *>(dataBase) + (_totalBlockSize * i));
            SetBlockHeader(static_cast<char *>(dataBase) + (_totalBlockSize * i), currentPageHeader);
        }

        ConstructPageFreeList(dataBase);
        ++_numPages;
    }

    // Link the page header to a block within the page.
    void SegmentedPoolAllocator::SetBlockHeader(void* blockAddress, void *pageHeader) const {
        // Set block header.
        static_cast<GenericObject*>(blockAddress)->_next = static_cast<GenericObject*>(pageHeader);
    }

    // Construct the free list of a newly allocated page.
    void SegmentedPoolAllocator::ConstructPageFreeList(void *dataBase) {
        // Construct free list.
        // Setup first block.
        auto* previousBlock = reinterpret_cast<GenericObject*>(static_cast<char*>(dataBase) + _numPaddingBytes + sizeof(void*));
        _freeList = previousBlock;

        // Set pointers to the rest of the blocks in the list.
        for (unsigned i = 1; i < _blocksPerPage; ++i) {
            auto* currentBlock = reinterpret_cast<GenericObject*>(reinterpret_cast<char*>(previousBlock) + (_totalBlockSize));
            previousBlock->_next = currentBlock;
            previousBlock = currentBlock;
        }

        previousBlock->_next = nullptr;
    }

    // Returns the memory held by empty/unused pages back to the OS.
    void SegmentedPoolAllocator::FreeEmptyPages() {
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

}
