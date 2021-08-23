
#ifndef SPARK_RING_BUFFER_TPP
#define SPARK_RING_BUFFER_TPP

namespace Spark::Job {

    template <typename T>
    RingBuffer<T>::RingBuffer(std::size_t capacity) : capacity_(capacity),
                                                      mask_(capacity_ - 1),
                                                      buffer_(new T[capacity_])
                                                      {
        SP_ASSERT(capacity_ > 0, "RingBuffer capacity must be greater than 0.");
        SP_ASSERT(!(capacity_ & (capacity_ - 1)), "RingBuffer capacity must be a power of 2.");
    }

    template <typename T>
    RingBuffer<T>::~RingBuffer() {
        delete[] buffer_;
    }

    template <typename T>
    void RingBuffer<T>::Store(std::size_t index, T&& object) {
        buffer_[index & mask_] = std::move(object);
    }

    template <typename T>
    T RingBuffer<T>::Load(std::size_t index) {
        return std::move(buffer_[index & mask_]);
    }

}

#endif //SPARK_RING_BUFFER_TPP
