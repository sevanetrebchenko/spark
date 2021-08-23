
#ifndef SPARK_JOB_SYSTEM_H
#define SPARK_JOB_SYSTEM_H

#include "spark/utility.h"
#include "spark/tools/singleton.h"
#include "spark/job/worker/worker.h"
#include "spark/job/job.h"
#include "spark/job/job_handle.h"
#include "spark/job/worker_pool.h"
#include "spark/job/job_handle_manager.h"

namespace Spark {
    namespace Job {

        class JobSystem : public Singleton<JobSystem> {
            public:
                REGISTER_SINGLETON(JobSystem);

                JobSystem();
                ~JobSystem() override;

                template <typename T, typename ...Args>
                JobHandleManager::ManagedJobHandle Schedule(Args&& ...args);

            private:
                template <typename Target, typename ...Types>
                void ValidateJobType() const;

                friend class Worker;
                NODISCARD const WorkerPool& GetWorkerPool() const;

                WorkerPool workerPool_;
                JobHandleManager jobHandleManager_;
        };

    }
}

#include "spark/job/job_system.tpp"

#endif //SPARK_JOB_SYSTEM_H
