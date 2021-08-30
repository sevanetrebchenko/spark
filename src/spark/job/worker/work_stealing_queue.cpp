
#include "spark/job/worker/work_stealing_queue.h"

namespace Spark::Job {

    WorkStealingQueue::WorkStealingQueue(std::size_t capacity) : capacity_(capacity),
                                                                 top_(0),
                                                                 bottom_(0),
                                                                 collection_(new JobEntry[capacity_])
                                                                 {
        SP_ASSERT(capacity_ > 0, "RingBuffer capacity must be greater than 0.");
        SP_ASSERT(!(capacity_ & (capacity_ - 1)), "RingBuffer capacity must be a power of 2.");
    }

    WorkStealingQueue::~WorkStealingQueue() {
        delete[] collection_;
    }

    // If bottom is greater than top, there are bottom - top elements in the queue.
    // If bottom is equal to or less than top, there are no elements in the queue.

    std::optional<JobEntry> WorkStealingQueue::Pop() {
        std::scoped_lock<std::mutex> lock(mutex_);
        int numJobs = bottom_ - top_;
        if (numJobs <= 0) {
            // Empty collection, nothing left to pop.
            return std::nullopt;
        }

        return { collection_[--bottom_ & mask_] };
    }

    std::optional<JobEntry> WorkStealingQueue::Steal() {
        std::scoped_lock<std::mutex> lock(mutex_);
        int numJobs = bottom_ - top_;
        if (numJobs <= 0) {
            // Empty collection, nothing left to steal.
            return std::nullopt;
        }

        std::optional<JobEntry> jobEntry = { collection_[top_++ & mask_] }; // Increment top.
        return jobEntry;
    }

    bool WorkStealingQueue::Push(JobHandle* handle, const JobVariant& jobVariant) {
        std::scoped_lock<std::mutex> lock(mutex_);
        // Construct object.
        collection_[bottom_++ & mask_] = std::make_pair<JobHandle*, JobVariant>(std::add_rvalue_reference<JobHandle*>::type(handle), std::add_rvalue_reference<JobVariant>::type(jobVariant) ); // Increment bottom.
        return true;
    }

}
