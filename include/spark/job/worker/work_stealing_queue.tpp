
#ifndef SPARK_WORK_STEALING_QUEUE_TPP
#define SPARK_WORK_STEALING_QUEUE_TPP

#include "spark/logger/logger.h"

namespace Spark::Job {

    template <typename T, typename... Args>
    bool WorkStealingQueue::Emplace(JobHandle* handle, Args&& ...args) {
        std::scoped_lock<std::mutex> lock(mutex_);
        // Construct object.
        collection_[bottom_++ & mask_] = std::make_pair<JobHandle*, JobVariant>(std::add_rvalue_reference<JobHandle*>::type(handle), JobVariant { std::forward<Args>(args)... } ); // Increment bottom.
        return true;
    }

}


#endif //SPARK_WORK_STEALING_QUEUE_TPP
