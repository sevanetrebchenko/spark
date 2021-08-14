#ifndef SPARK_MEMORY_RESOURCE_H
#define SPARK_MEMORY_RESOURCE_H

#include "spark/utility.h"

namespace Spark {
	namespace Memory {

		class IMemoryResource {
			public:
				IMemoryResource();
				virtual ~IMemoryResource();

				NODISCARD virtual void* allocate(std::size_t numBytes) = 0;
				virtual void deallocate(void* address, std::size_t numBytes) = 0;
		};

	}
}

#endif //SPARK_MEMORY_RESOURCE_H
