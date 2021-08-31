
#include "spark/job/job_system.h"

namespace Spark::Job {

    JobSystem::JobSystem() : workerPool_(4),
                             jobHandleManager_(workerPool_.GetCapacity() * WORKER_JOB_CAPACITY)
                             {
    }

    JobSystem::~JobSystem() {
        std::cout << "shutting down job system" << std::endl;
        workerPool_.Shutdown(); // Wait for all threads to finish before calling destructors.
    }

    const WorkerPool& JobSystem::GetWorkerPool() const {
        return workerPool_;
    }

}
