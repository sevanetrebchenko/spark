
#ifndef SPARK_UTILITY_TPP
#define SPARK_UTILITY_TPP

namespace Spark {

    namespace ECS::Internal {

        template <typename ComponentType>
        std::string CommaSeparatedList() {
            return std::string(ComponentType::Name);
        }

        template <typename ComponentType1, typename ComponentType2, typename... AdditionalComponentTypes>
        std::string CommaSeparatedList() {
            return CommaSeparatedList<ComponentType1>() + ", " + CommaSeparatedList<ComponentType2, AdditionalComponentTypes...>();
        }

    }

    namespace Internal {

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

    namespace Spark::ECS::Internal {



    }



}

#endif // SPARK_UTILITY_TPP