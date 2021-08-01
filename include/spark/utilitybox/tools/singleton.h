
#ifndef SPARK_SINGLETON_H
#define SPARK_SINGLETON_H

namespace Spark {

    template <typename Base>
    class Singleton {
        public:
            Singleton() = default;
            Singleton(Singleton const &other) = delete;
            Singleton(Singleton &&other) = delete;
            void operator=(Singleton const &other) = delete;

        protected:
            static Singleton* _instance;
    };

}

#endif //SPARK_SINGLETON_H
