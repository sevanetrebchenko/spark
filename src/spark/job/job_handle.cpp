
#include "spark/job/job_handle.h"
#include "spark/logger/logger.h"

namespace Spark::Job {

    JobHandle::JobHandle() : dependencies_(),
                             isComplete_(false),
                             isReady_(false),
                             isMarkedForReturn_(false)
                             {
    }

    JobHandle::~JobHandle() {
    }

    void JobHandle::Complete() {
        if (!isMarkedForReturn_) {
            // Begin execution on handle + dependencies.
            Stage();

            while (!isComplete_) {
                std::this_thread::yield();
            }
        }
        else {
            // Prevent additional Complete() calls on used JobHandle.
            LogWarning("Calling Complete on used JobHandle, operation does not do anything.");
        }
    }

    void JobHandle::Signal() {
        SP_ASSERT(!isMarkedForReturn_, "Sanity check - Signal called on already used JobHandle.");
        Reset(true);
    }

    void JobHandle::AddDependency(const ManagedJobHandle& managedJobHandle) {
        if (!isMarkedForReturn_) {
            JobHandle* jobHandle = managedJobHandle.get();
            dependencies_.emplace_back(jobHandle);
        }
        else {
            LogWarning("Calling AddDependency on used JobHandle, operation does not do anything.");
        }
    }

    const std::vector<JobHandle*>& JobHandle::GetDependencies() const {
        return dependencies_;
    }

    bool JobHandle::HasDependency() const {
        return !dependencies_.empty();
    }

    bool JobHandle::IsComplete() const {
        return isComplete_;
    }

    bool JobHandle::IsReady() const {
        return isReady_;
    }

    void JobHandle::Stage() {
        if (!isMarkedForReturn_) {
            // Mark all dependencies as ready for execution.
            for (JobHandle* dependency : dependencies_) {
                dependency->Stage();
            }

            // Mark this as ready for execution.
            isReady_.store(true);
        }
        else {
            LogWarning("Calling Stage on used JobHandle, operation does not do anything.");
        }
    }

    void JobHandle::Reset(bool markForReturn) {
        isComplete_ = false;
        isReady_ = false;
        isMarkedForReturn_ = markForReturn;

        // Clear dependency flags.
        for (JobHandle* dependency : dependencies_) {
            dependency->Reset(markForReturn);
        }

        // Clear dependencies.
        dependencies_.clear();
    }

}
