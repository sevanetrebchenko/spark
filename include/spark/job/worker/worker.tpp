
#ifndef SPARK_WORKER_TPP
#define SPARK_WORKER_TPP

namespace Spark::Job {

    template <typename T, typename ...Args>
    bool Worker::Schedule(JobHandle* handle, Args&& ...args) {
        return deque_.Emplace<T, Args...>(handle, std::forward<Args>(args)...);
    }

}

#endif //SPARK_WORKER_TPP
