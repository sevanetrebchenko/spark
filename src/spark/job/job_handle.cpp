
#include "spark/job/job_handle.h"
#include "spark/logger/logger.h"

namespace Spark::Job {

    JobHandle::JobHandle() : dependencies_(),
                             waitedOnComplete_(false),
                             isComplete_(false),
                             isReady_(false)
                             {
    }

    JobHandle::~JobHandle() {
    }

    void JobHandle::Complete() {
        if (!isComplete_) {
            // Begin execution on handle + dependencies.
            Stage();

            while (!isComplete_) {
                std::this_thread::yield();
            }

            waitedOnComplete_ = true;
            isComplete_ = true;
        }
        else {
            // Prevent additional Complete() calls on used JobHandle.
            LogWarning("Calling Complete on used JobHandle, operation does not do anything.");
        }
    }

    void JobHandle::Signal() {
        if (isComplete_) {
            std::cout << "used" << std::endl;
        }
//        SP_ASSERT(!isUsed_, "Sanity check - Signal called on already used JobHandle.");
        isComplete_ = true;
        isComplete_ = true;
    }

    void JobHandle::AddDependency(const ManagedJobHandle& managedJobHandle) {
        if (!isComplete_) {
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

    bool JobHandle::IsReady() const {
        return isReady_;
    }

    void JobHandle::Stage(const std::function<void(JobHandle*)>& callback) {
        if (!isComplete_) {
            // Mark all dependencies as ready for execution.
            for (JobHandle* dependency : dependencies_) {
                dependency->Stage(callback);
            }

            // Mark this as ready for execution.
            isReady_ = true;
        }
        else {
            LogWarning("Calling Stage on used JobHandle, operation does not do anything.");
        }
    }

    void JobHandle::Reset() {
        isComplete_ = false;
        isReady_ = false;

        // Clear dependency flags.
        for (JobHandle* dependency : dependencies_) {
            dependency->Reset();
        }

        // Clear dependencies.
        dependencies_.clear();
        waitedOnComplete_ = false;
    }

}
