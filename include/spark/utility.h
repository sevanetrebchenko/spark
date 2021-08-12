
#ifndef SPARK_UTILITY_H
#define SPARK_UTILITY_H

#pragma region Standard Includes
#include <cstdint>
#include <sstream>
#include <iostream>
#include <cstring>
#include <ostream>
#include <fstream>
#include <cstdio>
#include <functional>
#include <iomanip>
#include <cstdarg>
#include <utility>
#include <bitset>
#include <cmath>
#include <stdexcept>
#include <mutex>
#include <algorithm>
#include <cstdlib>
#include <atomic>
#include <thread>
#include <csignal>
#pragma endregion

#pragma region Standard Containers
#include <tuple>
#include <vector>
#include <unordered_map>
#include <queue>
#include <stack>
#include <string>
#include <unordered_set>
#include <forward_list>
#include <tuple>
#include <list>
#include <set>
#pragma endregion

// Enable asserts.
#define SP_ENABLE_ASSERTS

#ifdef SP_ENABLE_ASSERTS
    #include "spark/tools/assert_dev.h"
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

namespace Spark {

    // For use internally.
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

        #define STRINGHASH(string) (::Spark::Internal::FNV1a_32bit(string, sizeof(string) - 1))

    }

    // ECS utility functionality.
    namespace ECS {

        class IComponent; // Forward declaration.

        typedef std::uint32_t ComponentTypeID;
        typedef std::uint32_t EntityID;
        typedef std::unordered_map<ComponentTypeID, IComponent*> EntityComponentMap;

        namespace Internal {
            // Generates comma separated list of given components without trailing comma.
            template <typename ComponentType>
            std::string CommaSeparatedList();

            template <typename ComponentType1, typename ComponentType2, typename... AdditionalComponentTypes>
            std::string CommaSeparatedList();
        }

        #define INVALID_ID (EntityID)-1;
        #define REGISTER_COMPONENT(ClassName)                                           \
        public:                                                                         \
            static constexpr ::Spark::ECS::ComponentTypeID ID = STRINGHASH(#ClassName); \
            static constexpr const char* Name = #ClassName;                             \
        private:

    }

    // Events utility functionality.
    namespace Events {

        class IEvent; // Forward declaration.

        typedef std::uint32_t EventTypeID;
        typedef std::shared_ptr<const IEvent> EventHandle;

        namespace Internal {

            // Creates one virtual OnEvent function for each provided template argument.
            // Function signature is as follows:
            // void OnEvent(const GivenEventType*);
            template <typename... EventTypes>
            struct RequireOnEventForTypes {
                static_assert(sizeof...(EventTypes) > 0, "Invalid number template arguments provided to RequireOnEventForTypes.");
            };

            template <typename EventType, typename... AdditionalEventTypes>
            struct RequireOnEventForTypes<EventType, AdditionalEventTypes...> : RequireOnEventForTypes<EventType>, RequireOnEventForTypes<AdditionalEventTypes...> {
                using RequireOnEventForTypes<EventType>::OnEvent;
            };

            template <typename EventType>
            struct RequireOnEventForTypes<EventType> {
                virtual void OnEvent(const EventType* eventPointer) = 0;
            };

        }

        #define REGISTER_EVENT(ClassName)                                                \
        public:                                                                          \
            static constexpr ::Spark::Events::EventTypeID ID = STRINGHASH(#ClassName);   \
            static constexpr const char* Name = #ClassName;                              \
            NODISCARD ::Spark::Events::EventTypeID GetID() const override { return ID; }

    }

}

#include "spark/utility.tpp"

#endif //SPARK_UTILITY_H
