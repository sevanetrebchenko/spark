
#ifndef SPARK_SEGMENTED_POOL_ALLOCATOR_H
#define SPARK_SEGMENTED_POOL_ALLOCATOR_H

#include "spark/memory/allocator.h"
#include "spark/memory/resources/segmented_pool_resource.h"

namespace Spark {
	namespace Memory {

        template <typename T>
        class SegmentedPoolAllocator {//}; : public IAllocator<T, SegmentedPoolResource> {
            public:
                typedef T value_type;

                template <class U> explicit SegmentedPoolAllocator(SegmentedPoolAllocator<U> const&) noexcept {}

                SegmentedPoolAllocator();
                ~SegmentedPoolAllocator();

                T* allocate(std::size_t numObjects);
                void deallocate(T* address, std::size_t numObjects);
        };

    }
}

#include "spark/memory/allocators/segmented_pool_allocator.tpp"

#endif //SPARK_SEGMENTED_POOL_ALLOCATOR_H
