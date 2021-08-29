
#include "spark/job/job_handle_manager.h"
#include "spark/job/job_system.h"
#include "spark/logger/logger.h"

namespace Spark::Job {

    JobHandleManager::JobHandleManager(std::size_t capacity) : capacity_(capacity),
                                                               jobHandles_(new JobHandle[capacity])
                                                               {
        // Populate available index stack.
        for (unsigned index = capacity; index > 0; index--) {
            indices_.push(index - 1);
        }
    }

    JobHandleManager::~JobHandleManager() {
        delete[] jobHandles_;
    }

    ManagedJobHandle JobHandleManager::GetAvailableJobHandle() {
        // Collect completed handles.
        GarbageCollect();

        unsigned index = indices_.top();
        indices_.pop();

        ManagedJobHandle handle(&jobHandles_[index], ::Spark::Internal::CallbackFromMemberFn(this, &JobHandleManager::ReturnJobHandle));
        return handle;
    }

    void JobHandleManager::GarbageCollect() {
        for (unsigned i = 0; i < capacity_; ++i) {
            JobHandle& handle = jobHandles_[i];

            // Found handle marked for return.
            if (handle.markedForReturn_.load()) {
                handle.Reset(true); // Clear handle.
                indices_.push(i);
            }
        }
    }

    void JobHandleManager::ReturnJobHandle(JobHandle* jobHandle) {
        bool found = false;
        unsigned index = 0;

        for (unsigned i = 0; i < capacity_; ++i) {
            if (&jobHandles_[i] == jobHandle) {
                index = i;
                found = true;
                break;
            }
        }

        // Check for unregistered handles.
        if (!found) {
            LogWarning("JobHandleManager::ReturnJobHandle called with unregistered handle.");
            return;
        }

        jobHandle->Stage();
        return;

        // Dependencies will get scheduled through the parent.
        if (!jobHandle->IsDependency()) {

            // JobHandle can either be returned before or after job execution (depending on whether Complete() was used).
            if (jobHandle->isUsed_.load()) {
                // JobHandle has been used, job fully executed.
                jobHandle->Reset(true);
                indices_.push(index);
            }
            else {
                // JobHandle has not been used, job waiting to execute.

            }
        }
    }

}
