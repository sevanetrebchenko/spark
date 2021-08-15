
#include "spark/memory/memory_formatter.h"

namespace Spark::Memory {
    MemoryFormatter::MemoryFormatter(std::size_t blockDataSize, std::size_t numPaddingBytes) : userBlockDataSize_(blockDataSize),
                                                                                               numPaddingBytes_(numPaddingBytes) {
    }

    void MemoryFormatter::SetBlockMemorySignatures(void* blockAddress) const {
        unsigned offset = 0;

        // Set header memory signature.
        memset(static_cast<char*>(blockAddress) + offset, HEADER, sizeof(void*));
        offset += sizeof(void*);

        // Set padding memory signature.
        memset(static_cast<char*>(blockAddress) + offset, PADDING, numPaddingBytes_);
        offset += numPaddingBytes_;

        // Set unallocated memory signature.
        memset(static_cast<char*>(blockAddress) + offset, UNALLOCATED, userBlockDataSize_);
        offset += userBlockDataSize_;

        // Set padding memory signature.
        memset(static_cast<char*>(blockAddress) + offset, PADDING, numPaddingBytes_);
        offset += numPaddingBytes_;
    }

    void MemoryFormatter::SetBlockDataSignature(void *blockAddress, unsigned char memorySignature) const {
        memset(static_cast<char*>(blockAddress), memorySignature, userBlockDataSize_);
    }

    bool MemoryFormatter::ValidatePaddingBytes(void* blockAddress) const {
        // Check padding bytes before data.
        unsigned char* preBlockPaddingBytes = static_cast<unsigned char*>(blockAddress) - numPaddingBytes_;
        for (int i = 0; i < numPaddingBytes_; ++i) {
            if (preBlockPaddingBytes[i] != PADDING) {
                return false;
            }
        }

        // Check padding bytes after data.
        unsigned char* postBlockPaddingBytes = static_cast<unsigned char*>(blockAddress) + userBlockDataSize_;
        for (int i = 0; i < numPaddingBytes_; ++i) {
            if (postBlockPaddingBytes[i] != PADDING) {
                return false;
            }
        }

        return true;
    }

}

