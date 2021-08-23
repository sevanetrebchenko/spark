
#ifndef SPARK_JOB_HANDLE_MANAGER_H
#define SPARK_JOB_HANDLE_MANAGER_H

#include "spark/utility.h"
#include "spark/job/job_handle.h"

namespace Spark {
    namespace Job {

        class JobHandleManager {
            public:
                typedef std::unique_ptr<JobHandle, std::function<void(JobHandle*)>> ManagedJobHandle;

                explicit JobHandleManager(std::size_t capacity);
                ~JobHandleManager();

                NODISCARD ManagedJobHandle GetAvailableJobHandle();

            private:
                void ReturnJobHandle(JobHandle* jobHandle);

                std::size_t capacity_;
                JobHandle* jobHandles_;
                std::stack<unsigned> indices_;
        };

    }
}

#endif //SPARK_JOB_HANDLE_MANAGER_H
