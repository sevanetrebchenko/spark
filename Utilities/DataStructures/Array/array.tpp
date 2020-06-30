
#ifndef ARRAY_TPP
#define ARRAY_TPP

#include "../../assert_dev.h"

namespace UtilityBox::DataStructures {
    template<typename T>
    inline T* Array::operator[](unsigned int index) const {
        ASSERT(sizeof(T) == _blockSize, "Casting result of subscription operator to a type with a different size.");

        // Explicit static_cast conversion is valid with nullptr.
        return static_cast<T*>(_allocator[index]);
    }
}

#endif
