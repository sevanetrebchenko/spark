
#include "spark/job/worker_pool.h"

namespace Spark::Job {

    WorkerPool::WorkerPool(unsigned capacity) {
        unsigned hardware = std::thread::hardware_concurrency();
        if (hardware == 0) {
            // Threading unsupported.
            // Ignore capacity.
        }
        else {
            // Threading supported.
            workerCapacity_ = std::clamp(capacity, 0u, hardware - 1);
        }

        workers_ = new Worker[workerCapacity_];
    }

    WorkerPool::~WorkerPool() {
        delete[] workers_;
    }

    unsigned WorkerPool::GetCapacity() const {
        return workerCapacity_;
    }

    Worker* WorkerPool::GetRandomWorker() const {
        static thread_local std::random_device device;
        static thread_local std::mt19937 generator(device());
        std::uniform_int_distribution<int> distribution(0, static_cast<int>(workerCapacity_) - 1);

        return &workers_[distribution(generator)];
    }

    void WorkerPool::Shutdown() {
        for (unsigned i = 0; i < workerCapacity_; ++i) {
            workers_[i].Terminate();
        }
    }

}
