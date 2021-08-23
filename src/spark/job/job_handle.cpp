
#include "spark/job/job_handle.h"

namespace Spark::Job {

    JobHandle::JobHandle() : complete_(false) {
    }

    JobHandle::~JobHandle() {
    }

    void JobHandle::Complete() const {
        while (!complete_.load()) {
            std::this_thread::yield();
        }
    }

    void JobHandle::Signal() {
        complete_.store(true);
    }

    void JobHandle::Reset() {
        complete_.store(false);
    }

}
