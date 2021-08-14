
#ifndef SPARK_SEGMENTED_POOL_RESOURCE_H
#define SPARK_SEGMENTED_POOL_RESOURCE_H

#include "spark/memory/memory_resource.h"

namespace Spark {
	namespace Memory {

		class SegmentedPoolResource final : public IMemoryResource {
			public:
				SegmentedPoolResource(std::size_t blockSize, std::size_t blocksPerPage = 20, std::size_t numPaddingBytes = 4);
				~SegmentedPoolResource() override;

				NODISCARD void* allocate(std::size_t numBytes) override;
				void deallocate(void* address, std::size_t numBytes) override;

			private:
				std::size_t blockSize_;
				std::size_t blocksPerPage_;
				std::size_t numPaddingBytes_; // Padding bytes on either side of data block.
		};

	}
}

#endif //SPARK_SEGMENTED_POOL_RESOURCE_H
