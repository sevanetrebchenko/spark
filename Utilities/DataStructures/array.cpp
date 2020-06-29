
#ifndef DATASTRUCTURES_ARRAY_CPP
#define DATASTRUCTURES_ARRAY_CPP

#include "array.h"
#include "../assert_dev.h"

namespace UtilityBox::DataStructures {
    Array::Array(unsigned blockSize, unsigned numBlocks) : _blockSize(blockSize),
                                                           _numBlocks(numBlocks)
                                                           {
        // Nothing to do here.
    }

    void Array::Initialize() {
        _allocator.Initialize();
    }

    template <typename T>
    T* Array::operator[](unsigned int index) const {
        ASSERT(sizeof(T) == _blockSize, "Casting result of subscription operator to a type with a different size.");

        // Explicit static_cast conversion is valid with nullptr.
        return static_cast<T*>(_allocator[index]);
    }
}

#endif
