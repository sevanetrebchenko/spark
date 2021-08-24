
#include "spark/job/job_system.h"

namespace Spark::Job {

    JobSystem::JobSystem() : workerPool_(4),
                             jobHandleManager_(workerPool_.GetCapacity() * Worker::GetCapacity())
                             {
    }

    JobSystem::~JobSystem() {
        workerPool_.Shutdown(); // Wait for all threads to finish before calling destructors.
    }

    const WorkerPool& JobSystem::GetWorkerPool() const {
        return workerPool_;
    }

}
