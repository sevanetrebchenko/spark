
#ifndef SPARK_ALLOCATOR_H
#define SPARK_ALLOCATOR_H

#include "spark/utility.h"
#include "spark/memory/memory_resource.h"

namespace Spark {
	namespace Memory {

		// C++17 compliant allocator.
		template <typename T, typename MemoryResource>
		class IAllocator {
			public:
				typedef T value_type;

				IAllocator();
				virtual ~IAllocator();

				NODISCARD virtual constexpr void* allocate(std::size_t numBytes) = 0;
				virtual constexpr void deallocate(void* address, std::size_t numBytes) = 0;

				NODISCARD virtual bool operator==(const IAllocator& other) const = 0;
				NODISCARD virtual bool operator!=(const IAllocator& other) const = 0;

			protected:
				MemoryResource memoryResource_;
		};

	}
}

#endif //SPARK_ALLOCATOR_H
