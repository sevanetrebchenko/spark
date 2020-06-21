
#include <cstring>
#include "memory_manager.h"
#include "../assert_dev.h"

namespace UtilityBox {
    namespace Memory {
        class PoolAllocator::PoolAllocatorData {
            public:
                struct PageHeader {
                    PageHeader();
                    PageHeader* _next;
                    unsigned long _numInUse;
                };

                struct GenericObject {
                    GenericObject *_next;
                };

                explicit PoolAllocatorData(int blockSize);
                void Initialize();
                ~PoolAllocatorData();

                _NODISCARD_ void* RetrieveBlock();
                void ReturnBlock(void* blockAddress);

            private:
                void ConstructPage();
                void ConstructBlock(void* blockAddress, void* pageHeader) const;
                void ConstructFreeList(void* dataBase);
                bool ValidatePaddingBytes(void* blockAddress) const;
                void FreeEmptyPages();

                // Memory signatures.
                const unsigned char UNALLOCATED = 0xCC;
                const unsigned char ALLOCATED = 0xAA;
                const unsigned char PADDING = 0xFF;

                GenericObject* _freeList;
                PageHeader* _pageList;
                const unsigned _blocksPerPage = 10;
                const unsigned _numPaddingBytes = 4;
                unsigned _numPages;
                unsigned _blockDataSize;
                unsigned _totalBlockSize;
        };

        //--------------------------------------------------------------------------------------------------------------
        // PAGE HEADER
        //--------------------------------------------------------------------------------------------------------------
        PoolAllocator::PoolAllocatorData::PageHeader::PageHeader() : _numInUse(0), _next(nullptr) {
            // Nothing to do here.
        }

        //--------------------------------------------------------------------------------------------------------------
        // POOL ALLOCATOR DATA
        //--------------------------------------------------------------------------------------------------------------
        PoolAllocator::PoolAllocatorData::PoolAllocatorData(int blockSize) : _freeList(nullptr),
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

        void PoolAllocator::PoolAllocatorData::Initialize() {
            ConstructPage();
        }

        PoolAllocator::PoolAllocatorData::~PoolAllocatorData() {

        }

        void PoolAllocator::PoolAllocatorData::ConstructPage() {
            //                                                        |     SPACE FOR DATA     |
            // ___________________     _________________________________________________________________________
            // |                 |     |              |               |             |          |               |
            // |   PAGE HEADER   |  +  |    HEADER*   |    PADDING    |    NEXT*    |   ....   |    PADDING    |  x blocks per page
            // |_________________|     |______________|_______________|_____________|__________|_______________|
            //

            // ___________________
            // |                 |
            // |   PAGE HEADER   |
            // |_________________|

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
                pageList->_next = static_cast<PageHeader*>(pageBase);

                currentPageHeader = pageList->_next;
            }

            // Offset by the page header pointer to get to the first data block.
            void* dataBase = static_cast<char*>(pageBase) + sizeof(PageHeader);

            // Set memory signatures.
            for (int i = 0; i < _blocksPerPage; ++i) {
                ConstructBlock(static_cast<char*>(dataBase) + (_totalBlockSize * i), currentPageHeader);
            }

            ConstructFreeList(dataBase);

            ++_numPages;
        }

        void PoolAllocator::PoolAllocatorData::ConstructBlock(void* blockAddress, void* pageHeader) const {
            // Set block header.
            static_cast<GenericObject*>(blockAddress)->_next = static_cast<GenericObject*>(pageHeader);
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

        void PoolAllocator::PoolAllocatorData::ConstructFreeList(void *dataBase) {
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

        void PoolAllocator::PoolAllocatorData::FreeEmptyPages() {
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

        void* PoolAllocator::PoolAllocatorData::RetrieveBlock() {
            // Construct new page for blocks if there are no more remaining.
            if (!_freeList) {
                ConstructPage();
            }

            void* blockAddress = _freeList;
            auto* blockHeader = reinterpret_cast<PageHeader*>(reinterpret_cast<GenericObject*>(static_cast<char*>(blockAddress) - sizeof(void*) - _numPaddingBytes)->_next);
            ++blockHeader->_numInUse;
            _freeList = _freeList->_next;

            memset(blockAddress, ALLOCATED, _blockDataSize);
            return blockAddress;
        }

        void PoolAllocator::PoolAllocatorData::ReturnBlock(void *blockAddress) {
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

        bool PoolAllocator::PoolAllocatorData::ValidatePaddingBytes(void *blockAddress) const {
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

        //--------------------------------------------------------------------------------------------------------------
        // POOL ALLOCATOR
        //--------------------------------------------------------------------------------------------------------------
        PoolAllocator::PoolAllocator(int blockSize) : _blockSize(blockSize), _data(nullptr) {
            // Defer initialization until second stage.
        }

        void PoolAllocator::Initialize() {
            if (!_data) {
                _data = new PoolAllocatorData(_blockSize);
            }
            _data->Initialize();
        }

        void* PoolAllocator::RetrieveBlock() {
            return _data->RetrieveBlock();
        }

        void PoolAllocator::ReturnBlock(void *blockAddress) {
            _data->ReturnBlock(blockAddress);
        }
    }
}
