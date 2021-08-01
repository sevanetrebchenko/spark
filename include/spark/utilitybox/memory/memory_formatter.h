
#ifndef SPARK_MEMORY_FORMATTER_H
#define SPARK_MEMORY_FORMATTER_H

#include <spark/core/rename.h>

namespace Spark {
    namespace UtilityBox {
        namespace Memory {

            struct MemoryFormatter {
                explicit MemoryFormatter(unsigned blockDataSize, bool useBlockHeaders);
                ~MemoryFormatter() = default;

                void SetBlockMemorySignatures(void* blockAddress) const;
                void SetBlockDataSignature(void* blockAddress, unsigned char memorySignature) const;
                bool ValidatePaddingBytes(void* blockAddress) const;
                NODISCARD unsigned CalculateMemorySignatureBlockSize() const;

                const unsigned _numPaddingBytes = 4; // Number of padding bytes on either side of the user data block.
                unsigned _blockDataSize;             // Size of only the user data block.
                bool _useBlockHeaders;               // Flag whether allocator has headers for each block or not.

                // memory signatures.
                const unsigned char UNALLOCATED = 0xCC;
                const unsigned char ALLOCATED = 0xAA;
                const unsigned char HEADER = 0xBB;
                const unsigned char PADDING = 0xFF;
            };

        }
    }
}



#endif // SPARK_MEMORY_FORMATTER_H
