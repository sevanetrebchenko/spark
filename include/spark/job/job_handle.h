
#ifndef SPARK_JOB_HANDLE_H
#define SPARK_JOB_HANDLE_H

#include "spark/utility.h"
#include "spark/job/types/job.h"

namespace Spark {
    namespace Job {

        class JobHandle {
            public:
                JobHandle();
                ~JobHandle();

                // Blocks thread until job is complete.
                void Complete() const;

                // JobSystem signals job completion.
                void Signal();
                void Reset();

                // Job handles should not be copied.
                JobHandle& operator=(const JobHandle& other) = delete;
                JobHandle(const JobHandle& other) = delete;
                JobHandle(JobHandle&& other) = delete;

            private:
                std::atomic<bool> complete_;
        };

    }
}

#endif //SPARK_JOB_HANDLE_H
