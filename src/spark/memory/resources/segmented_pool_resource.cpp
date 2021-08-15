
#include "spark/memory/resources/segmented_pool_resource.h"
#include "spark/logger/logger.h"

namespace Spark::Memory {
    // Page layout is as follows:
    //                                                        |     SPACE FOR DATA     |
    // ___________________     _________________________________________________________________________
    // |                 |     |              |               |             |          |               |
    // |   PAGE HEADER   |  +  |    HEADER*   |    PADDING    |    NEXT*    |   ....   |    PADDING    |  x blocks per page
    // |_________________|     |______________|_______________|_____________|__________|_______________|
    SegmentedPoolResource::SegmentedPoolResource(std::size_t userDataBlockSize, std::size_t numBlocksPerPage) : userDataBlockSize_(userDataBlockSize < sizeof(void*) ? sizeof(void*) : userDataBlockSize), // User data block should be large enough to hold a pointer.
                                                                                                                numPaddingBytes_(4),
                                                                                                                totalBlockSize_(sizeof(void*) + 2 * numPaddingBytes_ + userDataBlockSize_),
                                                                                                                numBlocksPerPage_(numBlocksPerPage),
                                                                                                                numPages_(0),
                                                                                                                freeList_(nullptr),
                                                                                                                pageList_(nullptr),
                                                                                                                memoryFormatter_(userDataBlockSize_, numPaddingBytes_)
                                                                                                                {
        ConstructPage();
    }

    SegmentedPoolResource::~SegmentedPoolResource() {
        // Free all allocated pages.
        PageHeader* temp;

        while (pageList_) {
            temp = pageList_;
            pageList_ = pageList_->next_;
            free(temp);
        }
    }

    void* SegmentedPoolResource::allocate(std::size_t numObjects) {
        if (!freeList_) {
            ConstructPage();
        }

        void* blockAddress = freeList_;
        PageHeader* blockHeader = reinterpret_cast<PageHeader*>(reinterpret_cast<GenericObject*>(static_cast<char*>(blockAddress) - sizeof(void*) - numPaddingBytes_)->_next);
        ++blockHeader->numInUse_;

        // OnUpdate free list.
        freeList_ = freeList_->_next;

        memoryFormatter_.SetBlockDataSignature(blockAddress, MemoryFormatter::ALLOCATED);
        return blockAddress;
    }

    void SegmentedPoolResource::deallocate(void* address, std::size_t numObjects) {
        // Check padding bytes to ensure memory was not corrupted by data underflow/overflow.
        if (!memoryFormatter_.ValidatePaddingBytes(address)) {
            LogError("Failed to validate padding bytes of a returned block - memory corruption likely.");
            throw std::runtime_error("Failed to validate padding bytes of a returned block - memory corruption likely.");
        }

        memoryFormatter_.SetBlockDataSignature(address, MemoryFormatter::UNALLOCATED);

        PageHeader* blockHeader = reinterpret_cast<PageHeader*>(reinterpret_cast<GenericObject*>(static_cast<char*>(address) - sizeof(void*) - numPaddingBytes_)->_next);
        --blockHeader->numInUse_;

        // Add block to free list.
        GenericObject* previousFreeList = freeList_;
        freeList_ = static_cast<GenericObject*>(address);
        freeList_->_next = previousFreeList;
    }

    void SegmentedPoolResource::ConstructPage() {
        unsigned pageSize = sizeof(PageHeader) + (totalBlockSize_ * numBlocksPerPage_);

        // Allocate page.
        void* pageBase = malloc(pageSize);

        if (!pageBase) {
            throw std::bad_alloc();
        }

        PageHeader* currentPageHeader;

        if (!pageList_) {
            // Page list does not exist
            pageList_ = new (pageBase) PageHeader();
            currentPageHeader = pageList_;
        }
        else {
            PageHeader* pageList = pageList_;
            while (pageList->next_) {
                pageList = pageList->next_;
            }

            // Set the next page link.
            pageList->next_ = static_cast<PageHeader*>(pageBase);
            currentPageHeader = pageList->next_;
        }

        // Offset by the page header pointer to get to the first data block.
        void* dataBase = static_cast<char*>(pageBase) + sizeof(PageHeader);

        // Final block memory signatures and link header.
        for (unsigned i = 0; i < numBlocksPerPage_; ++i) {
            char* blockBase = static_cast<char *>(dataBase) + (totalBlockSize_ * i);
            memoryFormatter_.SetBlockMemorySignatures(blockBase);

            // Set block header.
            static_cast<GenericObject*>(static_cast<void*>(blockBase))->_next = static_cast<GenericObject*>(static_cast<void*>(currentPageHeader));
        }

        ConstructPageFreeList(dataBase);
        ++numPages_;
    }

    void SegmentedPoolResource::ConstructPageFreeList(void* dataBase) {
        // Get to the start of the user data block for the first block in this page.
        GenericObject* previousBlock = reinterpret_cast<GenericObject*>(static_cast<char*>(dataBase) + numPaddingBytes_ + sizeof(void*));
        freeList_ = previousBlock;

        // Set pointers to the rest of the blocks in the list.
        for (unsigned i = 1; i < numBlocksPerPage_; ++i) {
            GenericObject* currentBlock = reinterpret_cast<GenericObject*>(reinterpret_cast<char*>(previousBlock) + (totalBlockSize_));
            previousBlock->_next = currentBlock;
            previousBlock = currentBlock;
        }

        previousBlock->_next = nullptr;
    }

    void SegmentedPoolResource::FreeEmptyPages() {
        PageHeader* pageList = pageList_;
        PageHeader* previousPage = nullptr;
        while (pageList) {
            if (pageList->numInUse_ == 0) {
                if (previousPage) {
                    previousPage->next_ = pageList->next_;
                }
            }

            previousPage = pageList;
            pageList = pageList->next_;

            free(previousPage);
        }
    }

}
