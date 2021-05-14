
#ifndef SPARK_MEMORY_FORMATTER_H
#define SPARK_MEMORY_FORMATTER_H

#include <spark/core/core.h>

namespace Spark {
    namespace UtilityBox {
        namespace Memory {

            struct MemoryFormatter {
                /**
                * Constructor.
                * @param blockDataSize   - Size of user block of data.
                * @param useBlockHeaders - Flag if each block has a section for a header or not.
                */
                explicit MemoryFormatter(unsigned blockDataSize, bool useBlockHeaders);

                /**
                * Destructor.
                */
                ~MemoryFormatter() = default;

                /**
                * Set debug memory signatures for a block.
                * @param blockAddress - Block to set memory signature.
                */
                void SetBlockMemorySignatures(void* blockAddress) const;

                /**
                * Set debug memory signatures for a the user block data chunk.
                * @param blockAddress    - Address of block to set memory signature of.
                * @param memorySignature - memory signature to use.
                */
                void SetBlockDataSignature(void* blockAddress, unsigned char memorySignature) const;

                /**
                * Validate padding bytes on either side of the user data block to ensure no data corruption happened.
                * @param blockAddress - Pointer (provided to RetrieveBlock()) of block to check the padding bytes of.
                * @return On success: true (bytes are intact - no data corruption).
                *         On failure: false (bytes are modified - data corruption most likely occurred).
                */
                bool ValidatePaddingBytes(void* blockAddress) const;

                /**
                * Since user block can be less than the size of a pointer (as part of the required implementation by the
                * memory managers, there needs to be enough space to hold at least a pointer.
                * @return Size of the calculated user block data size.
                */
                _NODISCARD_ unsigned CalculateMemorySignatureBlockSize() const;

                const unsigned _numPaddingBytes = 4; // Number of padding bytes on either side of the user data block.
                unsigned _blockDataSize;             // Size of only the user data block.
                bool _useBlockHeaders;               // Flag whether allocator has headers for each block or not.

                // memory signatures.
                const unsigned char UNALLOCATED = 0xCC;
                const unsigned char ALLOCATED = 0xAA;
                const unsigned char HEADER = 0xBB;
                const unsigned char PADDING = 0xFF;
            };

        } // namespace Memory
    } // namespace UtilityBox
} // namespace Spark



#endif // SPARK_MEMORY_FORMATTER_H
