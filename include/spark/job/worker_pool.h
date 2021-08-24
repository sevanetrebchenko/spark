
#ifndef SPARK_WORKER_POOL_H
#define SPARK_WORKER_POOL_H

#include "spark/utility.h"
#include "spark/job/worker/worker.h"

namespace Spark {
    namespace Job {

        class WorkerPool {
            public:
                explicit WorkerPool(unsigned capacity = std::thread::hardware_concurrency());
                ~WorkerPool();

                void Shutdown();

                NODISCARD unsigned GetCapacity() const;
                NODISCARD Worker* GetRandomWorker() const;

            private:
                unsigned workerCapacity_;
                Worker* workers_;
        };

    }
}

#endif //SPARK_WORKER_POOL_H
