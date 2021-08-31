
#ifndef SPARK_WORK_STEALING_QUEUE_H
#define SPARK_WORK_STEALING_QUEUE_H

#include "spark/utility.h"
#include "spark/job/job_types.h"
#include "spark/job/job_handle.h"
#include "spark/job/job_definitions.h"

namespace Spark {
    namespace Job {

        class WorkStealingQueue {
            public:
                explicit WorkStealingQueue(std::size_t capacity);
                ~WorkStealingQueue();

                template <typename T, typename ...Args>
                bool Emplace(JobHandle* handle, Args&& ...args);
                bool Push(JobHandle* handle, const JobVariant& jobVariant);

                std::optional<JobEntry> Pop();
                std::optional<JobEntry> Steal();

            private:
                std::size_t capacity_;
                std::size_t mask_;

                JobEntry* collection_;
                std::mutex mutex_;
                int bottom_;
                int top_;
        };

    }
}

#include "spark/job/worker/work_stealing_queue.tpp"

#endif //SPARK_WORK_STEALING_QUEUE_H
