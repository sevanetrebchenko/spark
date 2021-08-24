
#ifndef SPARK_WORK_STEALING_QUEUE_TPP
#define SPARK_WORK_STEALING_QUEUE_TPP

#include "spark/logger/logger.h"

namespace Spark::Job {

    template <typename T, typename... Args>
    bool WorkStealingQueue::Emplace(JobHandle* handle, Args&& ...args) {
        std::int64_t bottom = bottom_.load(relaxed);
        std::int64_t top = top_.load(acquire);

        if ((bottom - top) + 1 > capacity_) {
            // Queue is full.
            LogWarning("Worker queue is at maximum capacity (%u).", capacity_);
            return false;
        }

        // Construct object.
        buffer_.Store(bottom, std::make_pair<JobHandle*, JobVariant>(std::add_rvalue_reference<JobHandle*>::type(handle), std::move(JobVariant { std::forward<Args>(args)... } )));

        std::atomic_thread_fence(release);
        bottom_.store(bottom + 1, relaxed);
        return true;
    }

}


#endif //SPARK_WORK_STEALING_QUEUE_TPP
