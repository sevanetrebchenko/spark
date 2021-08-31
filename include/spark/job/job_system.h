
#ifndef SPARK_JOB_SYSTEM_H
#define SPARK_JOB_SYSTEM_H

#include "spark/utility.h"
#include "spark/tools/singleton.h"
#include "spark/job/worker/worker.h"
#include "spark/job/types/job.h"
#include "spark/job/job_handle.h"
#include "spark/job/worker_pool.h"
#include "spark/job/job_handle_manager.h"
#include "spark/job/job_definitions.h"

namespace Spark {
    namespace Job {

        class JobSystem : public Singleton<JobSystem> {
            public:
                REGISTER_SINGLETON(JobSystem);

                JobSystem();
                ~JobSystem() override;

                // Job automatically gets scheduled for execution when JobHandle goes out of scope.
                // This allows for dependency setup between jobs without having to synchronize.
                template <typename T, typename ...Args>
                ManagedJobHandle Schedule(Args&& ...args);

            private:
                template <typename Target, typename ...Types>
                void ValidateJobType() const;

                void ReturnJobHandle(JobHandle* jobHandle);

                friend class Worker;
                NODISCARD const WorkerPool& GetWorkerPool() const;

                WorkerPool workerPool_;
                JobHandleManager jobHandleManager_;
        };

    }
}

#include "spark/job/job_system.tpp"

#endif //SPARK_JOB_SYSTEM_H
