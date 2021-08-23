
#include "spark/job/job_system.h"
#include "spark/logger/logger.h"

namespace Spark::Job {

    JobSystem::JobSystem() : workerPool_(),
                             jobHandleManager_(workerPool_.GetCapacity() * Worker::GetCapacity()){
    }

    JobSystem::~JobSystem() {
    }

    const WorkerPool& JobSystem::GetWorkerPool() const {
        return workerPool_;
    }

}
