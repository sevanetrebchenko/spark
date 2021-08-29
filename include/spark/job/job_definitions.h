
#ifndef SPARK_JOB_DEFINITIONS_H
#define SPARK_JOB_DEFINITIONS_H

#include "spark/utility.h"
#include "spark/job/job_types.h"

namespace Spark {
    namespace Job {

        class JobHandle;

        typedef std::variant<std::monostate, JOB_TYPES> JobVariant; // Allow default constructable std::variant via std::monostate.
        typedef std::pair<JobHandle*, JobVariant> JobEntry;
        typedef std::unique_ptr<JobHandle, std::function<void(JobHandle*)>> ManagedJobHandle;

        // Number of total jobs this queue can hold.
        #define WORKER_JOB_CAPACITY 4096

    }
}

#endif //SPARK_JOB_DEFINITIONS_H
