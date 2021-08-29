
#ifndef SPARK_JOB_SYSTEM_TPP
#define SPARK_JOB_SYSTEM_TPP

namespace Spark::Job {

    template <typename T, typename... Args>
    ManagedJobHandle JobSystem::Schedule(Args&& ...args) {
        ValidateJobType<T, JOB_TYPES>();
        Worker* worker = workerPool_.GetRandomWorker();

        ManagedJobHandle managedJobHandle = jobHandleManager_.GetAvailableJobHandle();
        worker->Schedule<T>(managedJobHandle.get(), std::forward<Args>(args)... );
        return managedJobHandle;
    }

    template <typename Target, typename... Types>
    void Job::JobSystem::ValidateJobType() const {
        static_assert((std::is_same_v<Target, Types> || ...), "Job type passed into JobSystem::Schedule must be registered in the JOB_TYPES macro.");
    }

}

#endif //SPARK_JOB_SYSTEM_TPP
