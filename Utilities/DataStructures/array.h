
#ifndef DATASTRUCTURES_ARRAY_H
#define DATASTRUCTURES_ARRAY_H

#include "../Memory/contiguous_pool_allocator.h"

namespace UtilityBox::DataStructures {
    class Array {
        public:
            Array(unsigned blockSize, unsigned numBlocks);
            void Initialize();
            ~Array() = default;

            template <typename T>
            T* operator[](unsigned index) const;

        private:
            unsigned _blockSize;
            unsigned _numBlocks;
            Memory::ContiguousPoolAllocator _allocator { _blockSize, _numBlocks, false };
    };
}

#include "array.cpp"
#endif //DATASTRUCTURES_ARRAY_H
