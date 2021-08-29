
#include "spark/job/job_handle.h"
#include "spark/logger/logger.h"

namespace Spark::Job {

    JobHandle::JobHandle() : dependencies_(),
                             isComplete_(false),
                             isReady_(false),
                             isDependency_(false),
                             markedForReturn_(false),
                             isUsed_(false)
                             {
    }

    JobHandle::~JobHandle() {
    }

    void JobHandle::Complete() {
        if (!isUsed_.load()) {
            // Begin execution on handle + dependencies.
            Stage();

            while (!isComplete_.load()) {
                std::this_thread::yield();
            }
        }
        else {
            // Prevent additional Complete() calls on used JobHandle.
            LogWarning("Calling Complete on used JobHandle, operation does not do anything.");
        }
    }

    void JobHandle::Signal() {
        SP_ASSERT(!isUsed_.load(), "Sanity check - Signal called on already used JobHandle.");
        isComplete_.store(true);
        markedForReturn_.store(true);
        isUsed_.store(true);
    }

    void JobHandle::AddDependency(const ManagedJobHandle& managedJobHandle) {
        if (!isUsed_.load()) {
            JobHandle* jobHandle = managedJobHandle.get();
            dependencies_.emplace_back(jobHandle);

            jobHandle->isDependency_.store(true);
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
        return isComplete_.load();
    }

    bool JobHandle::IsReady() const {
        return isReady_.load();
    }

    bool JobHandle::IsDependency() const {
        return isDependency_.load();
    }

    void JobHandle::Stage() {
        if (!isUsed_.load()) {
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

    void JobHandle::Reset(bool hard) {
        isComplete_.store(false);
        isReady_.store(false);
        markedForReturn_.store(false);

        if (hard) {
            isUsed_.store(false);
        }

        // Clear dependency flags.
        for (JobHandle* dependency : dependencies_) {
            dependency->Reset(hard);
        }

        // Clear dependencies.
        dependencies_.clear();
    }

}
