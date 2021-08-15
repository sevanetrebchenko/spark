
#ifndef SPARK_MEMORY_FORMATTER_H
#define SPARK_MEMORY_FORMATTER_H

#include "spark/utility.h"

namespace Spark {
    namespace Memory {

        class MemoryFormatter {
            public:
                static const unsigned char UNALLOCATED = 0xCC;
                static const unsigned char ALLOCATED = 0xAA;
                static const unsigned char HEADER = 0xBB;
                static const unsigned char PADDING = 0xFF;

                MemoryFormatter(std::size_t blockDataSize, std::size_t numPaddingBytes);
                ~MemoryFormatter() = default;

                void SetBlockMemorySignatures(void* blockAddress) const;

                // User data block gets set when retrieved/returned.
                void SetBlockDataSignature(void* blockAddress, unsigned char memorySignature) const;
                bool ValidatePaddingBytes(void* blockAddress) const;

            private:
                std::size_t userBlockDataSize_;
                std::size_t numPaddingBytes_;
        };

    }
}



#endif // SPARK_MEMORY_FORMATTER_H
