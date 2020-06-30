
#include "array.h"

namespace UtilityBox::DataStructures {
    Array::Array(unsigned blockSize, unsigned numBlocks) : _blockSize(blockSize),
                                                           _numBlocks(numBlocks)
                                                           {
        // Nothing to do here.
    }

    void Array::Initialize() {
        _allocator.Initialize();
    }
}
