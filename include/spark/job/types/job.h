
#ifndef SPARK_JOB_H
#define SPARK_JOB_H

namespace Spark {
    namespace Job {

        class IJob {
            public:
                IJob();
                virtual ~IJob();
                virtual void Execute() = 0;
        };

        class Test : public IJob {
            public:
                static std::atomic<int> counter;

                Test(int a){
                    a_ = a;
                }

                ~Test() override {
                }

                Test& operator=(const Test& rh) {
                    this->a_ = rh.a_;
                    return *this;
                }

                Test(const Test& Other) {
                    this->a_ = Other.a_;
                }

                Test(Test&& other) noexcept {
                    this->a_ = other.a_;
                }

                void Execute() override {
                    std::cerr << "sleeping..." << std::endl;
                    std::cerr << "Printing out Test job result: " << a_ << std::endl;
                    std::cerr.flush();

                    ++counter;
                }

            private:
                int a_;
        };

    }
}

#endif //SPARK_JOB_H
