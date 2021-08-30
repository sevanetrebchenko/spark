
#ifndef SPARK_WORKER_TPP
#define SPARK_WORKER_TPP

namespace Spark::Job {

    template <typename T, typename ...Args>
    void Worker::Schedule(JobHandle* handle, Args&& ...args) {
        deque_.Emplace<T>(handle, std::forward<Args>(args)...);
    }

}

#endif //SPARK_WORKER_TPP
