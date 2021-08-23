
#include "spark/job/worker/worker.h"
#include "spark/job/job_system.h"

// Number of total jobs this queue can hold.
#define CAPACITY 4096

namespace Spark::Job {

    std::size_t Worker::GetCapacity() {
        return CAPACITY;
    }

    Worker::Worker() : workerThreadActive_(true),
                       workerThread_(std::thread(&Worker::Distribute, this)),
                       deque_(CAPACITY)
                       {

    }

    Worker::~Worker() {
        workerThreadActive_ = false;
        workerThread_.join();
    }

    void Worker::Distribute() {
        while (workerThreadActive_) {
            std::optional<JobEntry> job = GetJob();
            job.has_value() ? ExecuteJob(job.value()) : std::this_thread::yield();
        }
    }

    std::optional<JobEntry> Worker::GetJob() {
        std::optional<JobEntry> job = deque_.Pop();

        // Pop from this worker queue was empty.
        if (!job.has_value()) {
            const WorkerPool& workerPool = Singleton<JobSystem>::GetInstance()->GetWorkerPool();
            Worker* worker = workerPool.GetRandomWorker();

            if (worker == this) {
                // Don't try to steal from this queue.
                return std::nullopt;
            }

            std::optional<JobEntry> stolenJob = worker->deque_.Steal();
            if (!stolenJob.has_value()) {
                // Failed to steal, yield and try again later.
                return std::nullopt;
            }

            return stolenJob;
        }

        return job.value();
    }

    void Worker::ExecuteJob(JobEntry& jobEntry) {
        if (std::monostate* mono = std::get_if<std::monostate>(&jobEntry.second)) {
            LogWarning("Entered ExecuteJob with no provided job (JobEntry contains std::monostate).");
            return;
        }

        // Calls the virtual Execute function on the job the JobEntry contains.
        std::visit(Internal::Visitor {
            [] (std::monostate& ms) {
                // Do nothing.
            },
            [] (auto& job) {
                job.Execute();
            }
        }, jobEntry.second);

        // Signal job completion.
        JobHandle* jobHandle = jobEntry.first;
        if (jobHandle) {
            jobHandle->Signal();
        }
    }

}
