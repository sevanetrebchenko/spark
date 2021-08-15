
#ifndef SPARK_ALLOCATOR_TPP
#define SPARK_ALLOCATOR_TPP

namespace Spark::Memory {

    template <typename T, class MemoryResource>
    IAllocator<T, MemoryResource>::~IAllocator() {
        delete memoryResource_;
    }

}

#endif //SPARK_ALLOCATOR_TPP