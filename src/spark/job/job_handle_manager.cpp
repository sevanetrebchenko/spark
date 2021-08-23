
#include "spark/job/job_handle_manager.h"
#include "spark/logger/logger.h"

namespace Spark::Job {

    JobHandleManager::JobHandleManager(std::size_t capacity) : capacity_(capacity),
                                                               jobHandles_(new JobHandle[capacity_])
                                                               {
        // Populate available index stack.
        for (unsigned index = capacity; index > 0; index--) {
            indices_.push(index - 1);
        }
    }

    JobHandleManager::~JobHandleManager() {
    }

    JobHandleManager::ManagedJobHandle JobHandleManager::GetAvailableJobHandle() {
        unsigned index = indices_.top();
        ManagedJobHandle handle(&jobHandles_[index], ::Spark::Internal::CallbackFromMemberFn(this, &JobHandleManager::ReturnJobHandle));
        indices_.pop();

        return std::move(handle);
    }

    void JobHandleManager::ReturnJobHandle(JobHandle* jobHandle) {
        bool found = false;
        unsigned index = 0;

        for (unsigned i = 0; i < capacity_; ++i) {
            if (&jobHandles_[i] == jobHandle) {
                jobHandles_[i].Reset();
                index = i;
                found = true;
                break;
            }
        }

        if (!found) {
            LogWarning("JobHandleManager::ReturnJobHandle called with unregistered handle.");
        }

        indices_.push(index); // Reuse this JobHandle.
        jobHandle = nullptr;
    }

}
