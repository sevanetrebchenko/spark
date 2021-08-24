
#ifndef SPARK_WORK_STEALING_QUEUE_H
#define SPARK_WORK_STEALING_QUEUE_H

#include "spark/job/job_types.h"
#include "spark/job/worker/ring_buffer.h"
#include "spark/job/job_handle.h"
#include "spark/job/job_definitions.h"

namespace Spark {
    namespace Job {

        class WorkStealingQueue {
            public:
                explicit WorkStealingQueue(std::size_t capacity);
                ~WorkStealingQueue();

                // Constructs a job in place.
                // Returns false if internal job buffer reached maximum capacity (job was not scheduled).
                // Note: Should ONLY be called by OWNING thread.
                template <typename T, typename ...Args>
                bool Emplace(JobHandle* handle, Args&& ...args);

                // Note: Should ONLY be called by OWNING thread.
                std::optional<JobEntry> Pop();

                // Note: Should ONLY be called by OTHER threads.
                std::optional<JobEntry> Steal();

            private:
                std::size_t capacity_;
                std::atomic<std::int64_t> top_;
                std::atomic<std::int64_t> bottom_;
                RingBuffer<JobEntry> buffer_; // Stores reference to JobHandle to signal job completion.

                // memory_order_acquire: Guarantees that subsequent loads are not moved before the current load or any preceding loads
                static constexpr std::memory_order acquire = std::memory_order_acquire;

                // memory_order_release: Preceding stores are not moved past the current store or any subsequent stores
                static constexpr std::memory_order release = std::memory_order_release;

                // memory_order_acq_rel: Combines memory_order_acquire and memory_order_release
                static constexpr std::memory_order acq_rel = std::memory_order_acq_rel;

                // memory_order_consume: Potentially weaker form of memory_order_acquire that enforces ordering of the current load before other operations that are data-dependent on it
                static constexpr std::memory_order consume = std::memory_order_consume;

                // memory_order_relaxed: All reorderings allowed
                static constexpr std::memory_order relaxed = std::memory_order_relaxed;

                // memory_order_seq_cst: Enforces full sequential consistency
                static constexpr std::memory_order seq_cst = std::memory_order_seq_cst;
        };

    }
}

#include "work_stealing_queue.tpp"

#endif //SPARK_WORK_STEALING_QUEUE_H
