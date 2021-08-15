
#ifndef SPARK_SEGMENTED_POOL_ALLOCATOR_TPP
#define SPARK_SEGMENTED_POOL_ALLOCATOR_TPP

namespace Spark::Memory {

	template <typename T>
	SegmentedPoolAllocator<T>::SegmentedPoolAllocator() {
//	    this->memoryResource_ = new SegmentedPoolResource(sizeof(T), 20);
	}

	template <typename T>
	SegmentedPoolAllocator<T>::~SegmentedPoolAllocator() {
	}

	template <typename T>
	T* SegmentedPoolAllocator<T>::allocate(std::size_t numObjects) {
	    void* addr = malloc(sizeof(T) * numObjects);
	    memset(addr, 0xFF, sizeof(T) * numObjects);
	    return static_cast<T*>(addr);
	}

	template <typename T>
	void SegmentedPoolAllocator<T>::deallocate(T* address, std::size_t numObjects) {
	    free(address);
	    //return this->memoryResource_->deallocate(address, numObjects);
	}

}

#endif //SPARK_SEGMENTED_POOL_ALLOCATOR_TPP
