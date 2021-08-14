
#ifndef SPARK_SEGMENTED_POOL_ALLOCATOR_TPP
#define SPARK_SEGMENTED_POOL_ALLOCATOR_TPP

namespace Spark::Memory {

	template <typename T>
	SegmentedPoolAllocator<T>::SegmentedPoolAllocator() {}

	template<typename T>
	SegmentedPoolAllocator<T>::~SegmentedPoolAllocator() {

	}

	template<typename T>
	void* SegmentedPoolAllocator<T>::allocate(std::size_t numBytes) {
		return this->memoryResource_.Alloc();
	}

	template<typename T>
	void SegmentedPoolAllocator<T>::deallocate(void* address, std::size_t numBytes) {

	}

	template<typename T>
	bool SegmentedPoolAllocator<T>::operator==(const SegmentedPoolAllocator& other) const {
		return false;
	}

	template<typename T>
	bool SegmentedPoolAllocator<T>::operator!=(const SegmentedPoolAllocator& other) const {
		return false;
	}

}

#endif //SPARK_SEGMENTED_POOL_ALLOCATOR_TPP
