
#include "spark/job/types/job.h"

namespace Spark::Job {

    IJob::IJob() = default;
    IJob::~IJob() = default;

    std::atomic<int> Test::counter = 0;

}