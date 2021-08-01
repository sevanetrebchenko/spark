
#include <spark/core/rename.h>
#include <spark/utilitybox/memory/memory_formatter.h> // MemoryFormatter

namespace Spark::UtilityBox::Memory {
    // Constructor.
    MemoryFormatter::MemoryFormatter(unsigned int blockDataSize, bool useBlockHeaders) : _blockDataSize(blockDataSize),
                                                                                         _useBlockHeaders(useBlockHeaders)
                                                                                         {
        // Nothing to do here.
    }

    // Set debug memory signatures for a block.
    void MemoryFormatter::SetBlockMemorySignatures(void *blockAddress) const {
        unsigned offset = 0;

        if (_useBlockHeaders) {
            // Set header memory signature.
            memset(static_cast<char*>(blockAddress) + offset, HEADER, sizeof(void*));
            offset += sizeof(void*);
        }

        // Set padding memory signature.
        memset(static_cast<char*>(blockAddress) + offset, PADDING, _numPaddingBytes);
        offset += _numPaddingBytes;

        // Set unallocated memory signature.
        unsigned dataBlockMemorySignatureSize = CalculateMemorySignatureBlockSize();
        memset(static_cast<char*>(blockAddress) + offset, UNALLOCATED, dataBlockMemorySignatureSize);
        offset += dataBlockMemorySignatureSize;

        // Set padding memory signature.
        memset(static_cast<char*>(blockAddress) + offset, PADDING, _numPaddingBytes);
        offset += _numPaddingBytes;
    }

    // Set debug memory signatures for a the user block data chunk.
    void MemoryFormatter::SetBlockDataSignature(void *blockAddress, unsigned char memorySignature) const {
        // Set data block memory signature.
        unsigned dataBlockMemorySignatureSize = CalculateMemorySignatureBlockSize();
        memset(static_cast<char*>(blockAddress), memorySignature, dataBlockMemorySignatureSize);
    }

    // Validate padding bytes on either side of the user data block to ensure no data corruption happened.
    bool MemoryFormatter::ValidatePaddingBytes(void *blockAddress) const {
        // Check padding bytes before data.
        unsigned char* preBlockPaddingBytes = static_cast<unsigned char*>(blockAddress) - _numPaddingBytes;
        for (int i = 0; i < _numPaddingBytes; ++i) {
            if (preBlockPaddingBytes[i] != PADDING) {
                return false;
            }
        }

        // Check padding bytes after data.
        unsigned char* postBlockPaddingBytes = static_cast<unsigned char*>(blockAddress) + _blockDataSize;
        for (int i = 0; i < _numPaddingBytes; ++i) {
            if (postBlockPaddingBytes[i] != PADDING) {
                return false;
            }
        }

        return true;
    }

    // Since user block can be less than the size of a pointer (as part of the required implementation by the memory
    // managers, there needs to be enough space to hold at least a pointer.
    unsigned MemoryFormatter::CalculateMemorySignatureBlockSize() const {
        // A block needs to at least be able to hold a pointer (if the data size is smaller than than of a pointer).

        // If size of data is larger than pointer, only use the size of the data.
        // Pointer (used for the free list) will be overwritten when used.
        return _blockDataSize >= sizeof(void*) ? _blockDataSize : sizeof(void*);
    }
}

