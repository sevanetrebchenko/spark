
#ifndef SPARK_SINGLETON_TPP
#define SPARK_SINGLETON_TPP

namespace Spark {

    template<typename Base>
    Base* Singleton<Base>::GetInstance() {
        // TODO: (Change?) Enforce default constructor requirement.
        static Base instance; // Scott Meyers singleton.
        return &instance;
    }

}

#endif //SPARK_SINGLETON_TPP