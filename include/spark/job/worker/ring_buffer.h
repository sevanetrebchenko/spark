
#ifndef SPARK_RING_BUFFER_H
#define SPARK_RING_BUFFER_H

#include "spark/utility.h"

namespace Spark {
    namespace Job {

        template <typename T>
        class RingBuffer {
            public:
                explicit RingBuffer(std::size_t capacity);
                ~RingBuffer();

                void Store(std::size_t index, T&& object);
                T Load(std::size_t index);

            private:
                std::size_t capacity_;
                std::size_t mask_;

                T* buffer_;
        };

    }
}

#include "ring_buffer.tpp"

#endif //SPARK_RING_BUFFER_H
