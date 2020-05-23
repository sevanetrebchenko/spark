#include "memory_manager.h"

class _MemoryManager {
    public:
        _NODISCARD_ static _MemoryManager *GetInstance();
        _NODISCARD_ void *allocate(const unsigned &, const std::string& = "Not provided.", const void * = nullptr);
        void deallocate(const void *location) noexcept;

    private:
        _MemoryManager();

        ~_MemoryManager() {};

        /**
         * Data blocks are structured as follows:
         *       __________________________________________________________
         *       |                   |       |                    |       |
         *       |   BLOCK *HEADER   | GUARD |     BLOCK DATA     | GUARD |
         *       |___________________|_______|____________________|_______|
         *
         * The block header keeps track of information about a specific block (information not stored within the block
         * itself).
         */
        struct MMBlockHeader { // 48 bytes
            MMBlockHeader(const std::string &label);

            ~MMBlockHeader();

            std::string _label;
            bool _inUse;
            unsigned _userBlockSize;
            void *_data;
        };

        struct MMPageHeader {
            explicit MMPageHeader(unsigned upperBound);

            ~MMPageHeader();

            unsigned _dataSize;
            MMBlockHeader *_blockHeaders;
            void *_dataPage;

            MMPageHeader *_next;
        };

        struct MMBucketHeader {
            MMBucketHeader(unsigned lowerBound, unsigned upperBound);

            ~MMBucketHeader();

            unsigned _numPages;
            std::pair<unsigned, unsigned> _blockSizeRange;
            unsigned _minDataSize;
            unsigned _maxDataSize;

            MMPageHeader *_pageList;
        };

        MMBucketHeader *_buckets;

        // enum for checking guard bytes after data was
        enum MemoryCheck {
            MM_CHECK_SUCCEEDED,
            MM_CHECK_FAILED
        };

        // FUNCTIONS
        MemoryCheck checkGuardBytes(const void *location) const;

        /**
         * Hashing function that gets the correct bucket to store the block into.
         * @param blockSize Block size to get the bucket of.
         * @return Index in the buckets vector to search for a block / append the new block to
         */
        _NODISCARD_ unsigned hash(const unsigned &blockSize) const;

        // DATA
        struct MemoryManagerConfiguration {
            MemoryManagerConfiguration();

            // Memory signatures for debug purposes
            const unsigned char UNALLOCATED = 0xFF;
            const unsigned char ALLOCATED = 0xCC;
            const unsigned char GUARD = 0xBB;
            const unsigned char ALIGNMENT = 0xAA;

            const unsigned PAGE_SIZE = 6144; // bytes in a page
            const unsigned numGuardBytes = 8; // 4 bytes on each side

            // buckets for various sized user data
            const unsigned numBuckets = 16;

            // array of bucket sizes
            // sized 16, 24, 32, 48, 64, 96, 128, 192, 256, 384, 512, 768, 1024, 1536, 2048, 3072 bytes
            unsigned *bucketSizes;
        };

        // Singleton
        static _MemoryManager *memoryManager;
        static MemoryManagerConfiguration *_config;
};

// Global static pointer to ensure singleton
_MemoryManager* _MemoryManager::memoryManager = nullptr;
_MemoryManager::MemoryManagerConfiguration* _MemoryManager::_config = nullptr;

// MEMORY MANAGER CONFIGURATION
_MemoryManager::MemoryManagerConfiguration::MemoryManagerConfiguration() {
    // allocate memory for buckets
    bucketSizes = static_cast<unsigned int *>(malloc(sizeof(unsigned) * numBuckets));

    // failed to allocate memory
    //ASSERT(ASSERT_LEVEL_FATAL, bucketSizes, "Memory manager failed to allocate memory for bucket sizes.");

    // fill data
    unsigned counter = 0;
    for (unsigned i = 4; i < 12; ++i) {
        bucketSizes[counter++] = (1u << i);
        bucketSizes[counter++] = 3 * (1u << (i - 1));
    }
}

// MEMORY MANAGER BUCKET HEADER
_MemoryManager::MMBucketHeader::MMBucketHeader(unsigned int lowerBound, unsigned int upperBound) : _numPages(1), _blockSizeRange(std::make_pair(lowerBound, upperBound)), _minDataSize(lowerBound - 16u), _maxDataSize(upperBound - 16u) {
    _pageList = new MMPageHeader(upperBound);
}

// MEMORY MANAGER PAGE HEADER
_MemoryManager::MMPageHeader::MMPageHeader(unsigned int upperBound) : _dataSize(upperBound) {
    // block sizes are designed to evenly split the page of memory
    //ASSERT(ASSERT_LEVEL_ERROR, _config->PAGE_SIZE % _dataSize == 0, "Page was not divided evenly without fragmentation with the given block size. Block size: %i", _dataSize);

    std::cout << "data size: " << _dataSize << std::endl;
    std::cout << "num headers: " << _config->PAGE_SIZE / _dataSize << std::endl << std::endl;

    // allocate block headers
    _blockHeaders = static_cast<MMBlockHeader *>(malloc(sizeof(MMBlockHeader) * _config->PAGE_SIZE / _dataSize));
}

// MEMORY MANAGER
_MemoryManager::_MemoryManager() {
    _config = new MemoryManagerConfiguration();

    // allocate buckets
    // TODO bounds checks on number of buckets
    _buckets = static_cast<MMBucketHeader*>(malloc(sizeof(MMBucketHeader) * (_config->numBuckets - 1)));

    // initialize bucket data using constructor / placement new into the above memory
//    UtilityBox::LoggingSystem::LogMessage(UtilityBox::LoggingSystem::DEBUG, "Allocating memory manager buckets.");

    int counter = 0;
    for (unsigned i = 1; i < _config->numBuckets; ++i) {
        new(&_buckets[i - 1]) MMBucketHeader(_config->bucketSizes[i - 1] + 1u, _config->bucketSizes[i]);
        ++counter;
    }

//    UtilityBox::LoggingSystem::LogMessage(UtilityBox::LoggingSystem::DEBUG, "Finished allocating %i buckets.", counter);

//    unsigned blockID = UtilityBox::LoggingSystem::GetOpenBlockID();
//    UtilityBox::LoggingSystem::StartMessageBlock(blockID);
    for (unsigned i = 0; i < _config->numBuckets - 1; ++i) {
//        UtilityBox::LoggingSystem::LogBlockMessage(blockID, UtilityBox::LoggingSystem::DEBUG, "Bucket ID: %i\nLower Bound: %i\nUpper Bound: %i\nMinimum/Maximum data size: %i/%i", i, _buckets[i]._blockSizeRange.first, _buckets[i]._blockSizeRange.second, _buckets[i]._minDataSize, _buckets[i]._maxDataSize);
    }
//    UtilityBox::LoggingSystem::EndMessageBlock(blockID);
}

_MemoryManager *_MemoryManager::GetInstance() {
    if (!memoryManager) {
        memoryManager = new _MemoryManager();
    }

    return memoryManager;
}

unsigned _MemoryManager::hash(const unsigned int &blockSize) const {
    unsigned begin = 0;
    unsigned end = _config->numBuckets - 1;
    unsigned middle = end / 2;

    // binary search
    while (begin <= end) {
        // found fitting bucket
        if (_buckets[middle]._minDataSize <= blockSize && _buckets[middle]._maxDataSize >= blockSize) {
            return middle;
        }

        if(_buckets[middle]._maxDataSize < blockSize) {
            begin = middle + 1;
        }
        else {
            end = middle - 1;
        }

        middle = (begin + end) / 2;
    }

    // bucket was not found, return 1 past the end
    return _config->numBuckets - 1;
}

_NODISCARD_ void *_MemoryManager::allocate(const unsigned int &blockSize, const std::string& label, const void *locationHint) {
    unsigned bucketID = hash(blockSize);
    //ASSERT(ASSERT_LEVEL_ERROR, bucketID < _config->numBuckets - 1, "message");

    return nullptr;
}


// PUBLIC API FUNCTION DEFINITIONS
namespace UtilityBox {
    namespace MemoryManager {
        void* allocate(const unsigned &blockSize, const std::string& label, const void* hint) {
            return _MemoryManager::GetInstance()->allocate(blockSize, label, hint);
        }

        void deallocate(void *location) noexcept {
            free(location);
        }
    }
}
