
#include "spark/job/worker/work_stealing_queue.h"

namespace Spark::Job {

    WorkStealingQueue::WorkStealingQueue(std::size_t capacity) : capacity_(capacity),
                                                                 top_(0),
                                                                 bottom_(0),
                                                                 buffer_(capacity_)
                                                                 {
    }

    WorkStealingQueue::~WorkStealingQueue() {
    }

    // Bottom always points to the next available slot in the circular buffer.
    // Top always points to the next element on top (for Steal).
    // Operations:
    // Push/Emplace:
    //   - Stores new job, increments bottom.
    // Pop:
    //   - Decrements bottom, returns job at old bottom position.
    // Steal:
    //   - Gets job at top, increments top.

    // If bottom is greater than top, there are bottom - top elements in the queue.
    // If bottom is equal to or less than top, there are no elements in the queue.

    std::optional<JobEntry> WorkStealingQueue::Pop() {
        // Adjust bottom index.
        std::int64_t bottom = bottom_.load(relaxed) - 1;
        bottom_.store(bottom, relaxed);

        std::atomic_thread_fence(seq_cst); // Barrier to ensure bottom gets updated before top is read.
        std::int64_t top = top_.load(relaxed);

        if (top <= bottom) {
            std::optional<JobEntry> jobEntry = buffer_.Load(bottom);

            if (top != bottom) {
                // More than 1 item left in the queue.
                return jobEntry;
            }

            // Buffer is not empty, one job remains (bottom was incremented at the start of the function).
            // Last job could get stolen by another worker.
            if (!top_.compare_exchange_strong(top, top + 1, seq_cst, relaxed)) {
                // top was incremented to match bottom + 1, there are no more elements in the array.
                // Failed race against Steal.
                return std::nullopt;
            }

            // Race successful against Steal, object will be popped off.
            bottom_.store(top + 1, relaxed);
            return jobEntry;
        }
        else {
            // Buffer is empty.
            bottom_.store(top, relaxed);
            return std::nullopt;
        }
    }

    std::optional<JobEntry> WorkStealingQueue::Steal() {
        std::int64_t top = top_.load(acquire);
        std::atomic_thread_fence(seq_cst); // Barrier to ensure top always gets read before bottom.
        std::int64_t bottom = bottom_.load(acquire);

        if (top < bottom) {
            // Job may be overwritten immediately after acquire happened.
            std::optional<JobEntry> jobEntry = { buffer_.Load(top) };

            if (!top_.compare_exchange_strong(top, top + 1, seq_cst, relaxed)) {
                // Concurrent steal operation removed this job.
                // Failed race against Steal.
                return std::nullopt;
            }

            return jobEntry;
        }
        else {
            // Buffer is empty, nothing to steal.
            return std::nullopt;
        }
    }

}
