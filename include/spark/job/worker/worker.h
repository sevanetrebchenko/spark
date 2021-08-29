
#ifndef SPARK_WORKER_H
#define SPARK_WORKER_H

#include "spark/utility.h"
#include "spark/job/worker/work_stealing_queue.h"
#include "spark/job/job_definitions.h"

namespace Spark {
    namespace Job {

        class Worker {
            public:
                Worker();
                ~Worker();

                void Terminate();

                template <typename T, typename... Args>
                void Schedule(JobHandle* jobHandle, Args&& ...args);

            private:
                void Distribute();

                std::optional<JobEntry> GetJob();
                void ExecuteJob(JobEntry& job);
                void ReturnJob(JobEntry& job);

                WorkStealingQueue deque_;
                std::atomic<bool> workerThreadActive_;
                std::thread workerThread_;
        };

    }
}

#include "worker.tpp"

#endif //SPARK_WORKER_H
