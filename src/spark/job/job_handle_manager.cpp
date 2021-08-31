
#include "spark/job/job_handle_manager.h"
#include "spark/job/job_system.h"
#include "spark/logger/logger.h"

namespace Spark::Job {

    JobHandleManager::JobHandleManager() {
	}

    JobHandleManager::~JobHandleManager() {
    }

    ManagedJobHandle JobHandleManager::GetAvailableJobHandle() {
        JobHandle* rawHandle = new JobHandle();
        validHandles_.emplace(rawHandle); // Register with manager.
        ManagedJobHandle managedHandle (rawHandle, ::Spark::Internal::CallbackFromMemberFn(this, &JobHandleManager::ReturnJobHandle));
        return managedHandle;
    }

    void JobHandleManager::ReturnJobHandle(JobHandle* jobHandle) {
        if (jobHandle->waitedOnComplete_) {
            // Job has been completed.
            auto jobHandleIter = validHandles_.find(jobHandle);
            if (jobHandleIter != validHandles_.end()) {
                validHandles_.erase(jobHandleIter);
            }

            delete jobHandle;
        }
        else {
            jobHandle->Stage();
        }
    }

}
