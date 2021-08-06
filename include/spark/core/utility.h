
#ifndef SPARK_CORE_UTILITY_H
#define SPARK_CORE_UTILITY_H

#include "spark/spark_pch.h"

// Enable asserts.
#define SP_ENABLE_ASSERTS

#ifdef SP_ENABLE_ASSERTS
    #include "spark/utilitybox/tools/assert_dev.h"
#else
    #define SP_ASSERT(check, formatString, ...)  \
    do {                                         \
        if (!(check)) {                          \
        }                                        \
    }                                            \
    while (false)
#endif

#define NODISCARD [[nodiscard]]
#define BIT_SHIFT(value) (1u << (value))
#define PARAMETER_PACK_EXPAND(function, args, ...) ((void)function<args>(__VA_ARGS__), ...);
#define STRINGHASH(string) (Spark::Internal::FNV1a_32bit(string, sizeof(string) - 1))

namespace Spark {
    namespace Internal {

        // With configurable return value.
        template <class Class, typename ReturnType, typename ...FunctionArguments>
        NODISCARD inline auto CallbackFromMemberFn(Class* classInstance, ReturnType(Class::*memberFunction)(FunctionArguments...));

        template <class Class, class ...FunctionArgument>
        NODISCARD inline auto CallbackFromMemberFn(Class* classInstance, void(Class::*memberFunction)(FunctionArgument...));

        NODISCARD constexpr std::uint32_t FNV1a_32bit(const char *string, std::size_t currentIndex);

        void ConvertToLowercase(std::string& string);

        // Case agnostic string compare.
        NODISCARD bool StringCompare(const std::string& first, const std::string& second);

        // Unique types.
        template <class... Type>
        struct RequireUniqueTypes {
        };

        template <typename T1, typename T2, typename... Types>
        struct RequireUniqueTypes<T1, T2, Types...> : RequireUniqueTypes<T1, T2>, RequireUniqueTypes<T1, Types...>, RequireUniqueTypes<T2, Types...> {
        };

        template <typename T1, typename T2>
        struct RequireUniqueTypes<T1, T2> {
            static_assert(!std::is_same<T1, T2>::value, "Template parameters must be unique from other template parameters.");
        };

    }
}

#include "spark/core/utility.tpp"

#endif //SPARK_CORE_UTILITY_H
