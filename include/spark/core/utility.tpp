
#ifndef SPARK_CORE_UTILITY_TPP
#define SPARK_CORE_UTILITY_TPP

namespace Spark::Internal {

    // With configurable return value.
    template <class Class, typename ReturnType, typename ...FunctionArguments>
    inline auto CallbackFromMemberFn(Class* classInstance, ReturnType(Class::*memberFunction)(FunctionArguments...)) {
        return [classInstance, memberFunction](FunctionArguments... additionalArguments) {
            // 'this' argument gets passed implicitly into member functions. Mimic this same behavior explicitly
            // to be able to call the class member function directly.
            (ReturnType)(std::mem_fn(memberFunction)(classInstance, additionalArguments...));
        };
    }

    template <class Class, class ...FunctionArgument>
    inline auto CallbackFromMemberFn(Class* classInstance, void(Class::*memberFunction)(FunctionArgument...)) {
        return [classInstance, memberFunction](FunctionArgument... additionalArgument) {
            // 'this' argument gets passed implicitly into member functions. Mimic this same behavior explicitly
            // to be able to call the class member function directly.
            (void)(std::mem_fn(memberFunction)(classInstance, additionalArgument...));
        };
    }

    constexpr std::uint32_t FNV1a_32bit(const char *string, std::size_t currentIndex) {
        if (currentIndex) {
            return (FNV1a_32bit(string, currentIndex - 1u) ^ static_cast<unsigned char>(string[currentIndex])) * 16777619u;
        }
        else {
            return (2166136261u ^ static_cast<unsigned char>(string[currentIndex])) * 16777619u;
        }
    }

}

#endif // SPARK_CORE_UTILITY_TPP