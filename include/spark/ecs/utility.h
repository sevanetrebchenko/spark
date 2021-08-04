
#ifndef SPARK_ECS_UTILITY_H
#define SPARK_ECS_UTILITY_H

#include <spark/spark_pch.h>
#include <spark/core/rename.h>

namespace Spark {
    namespace ECS {

        struct BaseComponent;

        typedef std::uint32_t ComponentTypeID;
        typedef std::uint32_t EntityID;
        typedef std::unordered_map<ComponentTypeID, BaseComponent*> EntityComponentMap;

        // Case agnostic string equality operator.
        NODISCARD bool StringCompare(const std::string& first, const std::string& second);
        NODISCARD EntityID GetEntityIDFromName(const std::string& entityName);

        namespace Internal {
            // Generates comma separated list of given components without trailing comma.
            template <typename ComponentType>
            std::string CommaSeparatedList();

            template <typename ComponentType1, typename ComponentType2, typename... AdditionalComponentTypes>
            std::string CommaSeparatedList();
        }

    }
}

#include <spark/ecs/utility.tpp>

#define REGISTER_COMPONENT(ClassName)                                         \
public:                                                                       \
    static constexpr Spark::ECS::ComponentTypeID ID = STRINGHASH(#ClassName); \
    static constexpr const char* Name = #ClassName;                           \
private:

#endif //SPARK_ECS_UTILITY_H
