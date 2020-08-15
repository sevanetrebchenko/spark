
#ifndef SPARK_UTILITY_FUNCTIONS_H
#define SPARK_UTILITY_FUNCTIONS_H

namespace Spark {

    template <class Class, typename ReturnType, typename ...FunctionArguments>
    static auto CallbackFromMemberFn(Class* classInstance, ReturnType(Class::*memberFunction)(FunctionArguments...)) {
        return [classInstance, memberFunction](FunctionArguments... additionalArguments) {
            // 'this' argument gets passed implicitly into member functions. Mimic this same behavior explicitly
            // to be able to call the class member function directly.
            (ReturnType)(std::mem_fn(memberFunction)(classInstance, additionalArguments...));
        };
    }

}

#endif //SPARK_UTILITY_FUNCTIONS_H
