
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

        if (indices_.empty()) {
        	throw std::runtime_error("uh oh");
        }

        unsigned index = indices_.top();
        indices_.pop();

        ManagedJobHandle handle(&jobHandles_[index], ::Spark::Internal::CallbackFromMemberFn(this, &JobHandleManager::ReturnJobHandle));
        return handle;
    }

    void JobHandleManager::GarbageCollect() {
        for (unsigned i = 0; i < capacity_; ++i) {
            JobHandle& handle = jobHandles_[i];

            // Found handle marked for return.
            if (handle.isMarkedForReturn_.load()) {
                handle.Reset(false); // Clear handle.
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

        // Job handle has been completed already.
        if (jobHandle->isMarkedForReturn_) {
        	// JobHandle has been used, job fully executed.
        	jobHandle->Reset(false);
        	indices_.push(index);
        }
        else {
        	jobHandle->Stage();
        }
    }

}
