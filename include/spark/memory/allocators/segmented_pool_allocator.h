
#ifndef SPARK_SEGMENTED_POOL_ALLOCATOR_H
#define SPARK_SEGMENTED_POOL_ALLOCATOR_H

#include "spark/memory/allocator.h"
#include "spark/memory/resources/segmented_pool_resource.h"

namespace Spark {
	namespace Memory {

		template <typename T>
		class SegmentedPoolAllocator final : public IAllocator<T, SegmentedPoolResource> {
			public:
				SegmentedPoolAllocator();
				~SegmentedPoolAllocator() override;

				NODISCARD void* allocate(std::size_t numBytes) override;
				void deallocate(void* address, std::size_t numBytes) override;

				NODISCARD bool operator==(const SegmentedPoolAllocator& other) const override;
				NODISCARD bool operator!=(const SegmentedPoolAllocator& other) const override;
		};

	}
}

#include "spark/memory/allocators/segmented_pool_allocator.tpp"

#endif //SPARK_SEGMENTED_POOL_ALLOCATOR_H
