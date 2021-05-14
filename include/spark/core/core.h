
#ifndef SPARK_CORE_H
#define SPARK_CORE_H

#include <spark/spark_pch.h>
#include <spark/core/platform_detection.h>

#ifndef NDEBUG
    #define SP_DEBUG
#endif

// Enable debug builds.
#ifdef SP_DEBUG
    // Enable debug breaks.
    #if defined(SP_PLATFORM_LINUX)
        #include <csignal>
        #define SP_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Debug breaks are not suppported on the current platform."
    #endif

    #define SP_ENABLE_ASSERTS
#else
    // Does nothing.
    #define SP_DEBUGBREAK()
#endif

// Enable asserts
#ifdef SP_ENABLE_ASSERTS
    #include <spark/utilitybox/tools/assert_dev.h>
#else
    #define SP_ASSERT(check, formatString, ...)  \
    do {                                         \
        if (!(check)) {                          \
        }                                        \
    }                                            \
    while (false)
#endif

namespace Spark {

//    /**
//     * Construct a standalone function pointer from a class member function.
//     * @tparam Class              - Class holding the member function.
//     * @tparam ReturnType         - Member function return type.
//     * @tparam FunctionArguments  - Member function parameters.
//     * @param classInstance       - Class instance / 'this' pointer.
//     * @param memberFunction      - Pointer to member function.
//     * @return Member function callable from outside of the class.
//     */
//    template <class Class, typename ReturnType, typename ...FunctionArguments>
//    inline auto CallbackFromMemberFn(Class* classInstance, ReturnType(Class::*memberFunction)(FunctionArguments...)) {
//        return [classInstance, memberFunction](FunctionArguments... additionalArguments) {
//            // 'this' argument gets passed implicitly into member functions. Mimic this same behavior explicitly
//            // to be able to call the class member function directly.
//            (ReturnType)(std::mem_fn(memberFunction)(classInstance, additionalArguments...));
//        };
//    }

    template <class Class, class FunctionArgument>
    inline auto CallbackFromMemberFn(Class* classInstance, void(Class::*memberFunction)(FunctionArgument)) {
        return [classInstance, memberFunction](FunctionArgument additionalArgument) {
            // 'this' argument gets passed implicitly into member functions. Mimic this same behavior explicitly
            // to be able to call the class member function directly.
            (void)(std::mem_fn(memberFunction)(classInstance, additionalArgument));
        };
    }

    /**
     * FNV-1a 32-bit hashing algorithm.
     * @param string       - String to hash.
     * @param currentIndex - Current character index of the string while hashing.
     * @return
     */
    constexpr inline std::uint32_t FNV1a_32bit(const char *string, std::size_t currentIndex) {
        if (currentIndex) {
            return (FNV1a_32bit(string, currentIndex - 1u) ^ static_cast<unsigned char>(string[currentIndex])) * 16777619u;
        }
        else {
            return (2166136261u ^ static_cast<unsigned char>(string[currentIndex])) * 16777619u;
        }
    }

    /**
     * Convert a given string to all lowercase.
     * @param string - String to convert.
     */
    inline void ConvertToLowercase(std::string& string) {
        // Character by character traversal + conversion.
        for (char& character : string) {
            character = static_cast<char>(tolower(character));
        }
    }

}

#define _NODISCARD_ [[nodiscard]]
#define BIT_SHIFT(value) (1u << value)
#define PARAMETER_PACK_EXPAND(function, args, ...) ((void)function<args>(__VA_ARGS__), ...);
#define STRINGHASH(string) (Spark::FNV1a_32bit(string, sizeof(string) - 1))

#endif // SPARK_CORE_H
