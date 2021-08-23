
#ifndef SPARK_WORKER_H
#define SPARK_WORKER_H

#include "spark/utility.h"
#include "work_stealing_queue.h"

namespace Spark {
    namespace Job {

        class Worker {
            public:
                static std::size_t GetCapacity();

                Worker();
                ~Worker();

                template <typename T, typename ...Args>
                bool Schedule(JobHandle* handle, Args&& ...args);

            private:
                void Distribute();

                std::optional<JobEntry> GetJob();
                void ExecuteJob(JobEntry& job);

                WorkStealingQueue deque_;

                std::atomic<bool> workerThreadActive_;
                std::thread workerThread_;
        };

    }
}

#include "worker.tpp"

#endif //SPARK_WORKER_H
