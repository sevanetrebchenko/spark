
#ifndef SPARK_SINGLETON_H
#define SPARK_SINGLETON_H

namespace Spark {

    template <typename Base>
    class Singleton {
        public:
            static Base* GetInstance();

            // Singleton should not be copied/duplicated.
            Singleton(Singleton const &other) = delete;
            Singleton(Singleton &&other) = delete;
            void operator=(Singleton const &other) = delete;

        protected:
            Singleton() = default;
            virtual ~Singleton() = default;
    };

}

#include "spark/tools/singleton.tpp"

// Must be included with every singleton instance.
#define REGISTER_SINGLETON(Base) friend class ::Spark::Singleton<Base>;

#endif //SPARK_SINGLETON_H
