
#ifndef SPARK_ALLOCATOR_H
#define SPARK_ALLOCATOR_H

#include "spark/utility.h"
#include "spark/memory/memory_resource.h"

namespace Spark {
	namespace Memory {

		// C++17 compliant allocator.
		template <typename T, class MemoryResource>
		class IAllocator {
			public:
				typedef T value_type;

				IAllocator() = default;
				virtual ~IAllocator();

				NODISCARD virtual constexpr T* allocate(std::size_t numBytes) = 0;
				virtual constexpr void deallocate(T* address, std::size_t numBytes) = 0;

			protected:
				MemoryResource* memoryResource_;
		};

	}
}

#include "spark/memory/allocator.tpp"

#endif //SPARK_ALLOCATOR_H
