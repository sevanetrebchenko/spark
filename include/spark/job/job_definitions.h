
#ifndef SPARK_JOB_DEFINITIONS_H
#define SPARK_JOB_DEFINITIONS_H

#include "spark/utility.h"
#include "spark/job/job_types.h"
#include "spark/job/job_handle.h"

namespace Spark {
    namespace Job {

        typedef std::variant<std::monostate, JOB_TYPES> JobVariant; // Allow default constructable std::variant via std::monostate.
        typedef std::pair<JobHandle*, JobVariant> JobEntry;
        typedef std::unique_ptr<JobHandle, std::function<void(JobHandle*)>> ManagedJobHandle;

    }
}

#endif //SPARK_JOB_DEFINITIONS_H
