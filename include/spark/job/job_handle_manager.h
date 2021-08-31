
#ifndef SPARK_JOB_HANDLE_MANAGER_H
#define SPARK_JOB_HANDLE_MANAGER_H

#include "spark/utility.h"
#include "spark/job/job_handle.h"

namespace Spark {
    namespace Job {

        class JobHandleManager {
            public:
                JobHandleManager();
                ~JobHandleManager();

                NODISCARD ManagedJobHandle GetAvailableJobHandle();

            private:
                void ReturnJobHandle(JobHandle* jobHandle);

                std::unordered_set<JobHandle*> validHandles_;
        };

    }
}

#endif //SPARK_JOB_HANDLE_MANAGER_H
