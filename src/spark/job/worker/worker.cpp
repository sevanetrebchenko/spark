
#include "spark/job/worker/worker.h"
#include "spark/job/job_system.h"

namespace Spark::Job {

    Worker::Worker() : workerThreadActive_(true),
                       workerThread_(std::thread(&Worker::Distribute, this)),
                       deque_(WORKER_JOB_CAPACITY)
                       {

    }

    Worker::~Worker() {
    }

    void Worker::Distribute() {
        while (workerThreadActive_.load()) {
            std::optional<JobEntry> job = GetJob();

            if (job.has_value()) {
                JobHandle* jobHandle = job->first;

                // Don't execute job if job is not staged for execution.
                while (!jobHandle->IsReady()) {
                    std::cout << "job not ready" << std::endl;
                    std::cout.flush();
                    std::this_thread::yield();
                }

                // Don't execute job if dependencies are not complete.
                for (const JobHandle* dependency : jobHandle->GetDependencies()) {
                    if (!dependency->IsComplete()) {
                        std::cout << "dependency not ready" << std::endl;
                        std::cout.flush();
                        std::this_thread::yield();
                    }
                }

                std::cout << this << " executing" << std::endl;
                ExecuteJob(job.value());
            }

            //std::this_thread::sleep_for(std::chrono::milliseconds(200));
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

            std::cout << this << " stole from " << worker << std::endl;
            return stolenJob;
        }

        std::cout << this << " popped" << std::endl;
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

    void Worker::Terminate() {
        workerThreadActive_.store(false);
        SP_ASSERT(workerThread_.joinable(), "Thread is not joinable.");
        workerThread_.join();

        std::cout << "terminating" << std::endl;
        deque_.Print();
    }

}
