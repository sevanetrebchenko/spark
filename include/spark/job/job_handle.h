
#ifndef SPARK_JOB_HANDLE_H
#define SPARK_JOB_HANDLE_H

#include "spark/utility.h"
#include "spark/job/types/job.h"
#include "spark/job/job_definitions.h"

namespace Spark {
    namespace Job {

        class JobHandle {
            public:
                JobHandle();
                ~JobHandle();

                void AddDependency(const ManagedJobHandle& managedJobHandle);
                NODISCARD bool HasDependency() const;

                // Blocks thread until job is complete.
                void Complete();

                NODISCARD bool IsComplete() const;
                NODISCARD bool IsReady() const;
                NODISCARD bool IsDependency() const;

                // Job handles should not be copied.
                JobHandle& operator=(const JobHandle& other) = delete;
                JobHandle(const JobHandle& other) = delete;
                JobHandle(JobHandle&& other) = delete;

            private:
                friend class JobHandleManager;
                // Sets this and all dependencies to be ready for execution.
                void Stage();

                // Reset handle to default values for reuse.
                // Hard reset clears used flag.
                void Reset(bool hard);

                friend class Worker;
                void Signal();
                NODISCARD const std::vector<JobHandle*>& GetDependencies() const;

                std::vector<JobHandle*> dependencies_;
                std::atomic<bool> isComplete_;
                std::atomic<bool> isReady_;
                std::atomic<bool> isDependency_;
                std::atomic<bool> markedForReturn_;

                std::atomic<bool> isUsed_;
        };

    }
}

#endif //SPARK_JOB_HANDLE_H
