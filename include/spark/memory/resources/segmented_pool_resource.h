
#ifndef SPARK_SEGMENTED_POOL_RESOURCE_H
#define SPARK_SEGMENTED_POOL_RESOURCE_H

#include "spark/memory/memory_resource.h"
#include "spark/memory/memory_formatter.h"

namespace Spark {
	namespace Memory {

		class SegmentedPoolResource final : public IMemoryResource {
			public:
				SegmentedPoolResource(std::size_t userDataBlockSize, std::size_t numBlocksPerPage);
				~SegmentedPoolResource() override;

				NODISCARD void* allocate(std::size_t numBytes) override;
				void deallocate(void* address, std::size_t numBytes) override;

			private:
		        // Holds data about the blocks in one specific page. Creates a linked list with pages.
		        struct PageHeader {
		            PageHeader* next_ = nullptr; // Next page pointer.
                    unsigned numInUse_ = 0;      // Number of blocks that are in use in this page.
		        };

		        struct GenericObject {
		            GenericObject* _next = nullptr;
		        };

		        void ConstructPage();
		        void ConstructPageFreeList(void* dataBase);
		        void FreeEmptyPages();

		        std::size_t userDataBlockSize_;
		        std::size_t numPaddingBytes_;
		        std::size_t totalBlockSize_;

		        std::size_t numBlocksPerPage_;
		        std::size_t numPages_;

		        GenericObject* freeList_;
		        PageHeader* pageList_;
		        MemoryFormatter memoryFormatter_;
		};

	}
}

#endif //SPARK_SEGMENTED_POOL_RESOURCE_H
