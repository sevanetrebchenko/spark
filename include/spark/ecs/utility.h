
#ifndef SPARK_ECS_UTILITY_H
#define SPARK_ECS_UTILITY_H

#include "spark/core/utility.h"
#include "spark/ecs/components/types/base_component.h"

namespace Spark {
    namespace ECS {

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

    }
}

#include "spark/ecs/utility.tpp"

#define INVALID_ID (EntityID)-1;
#define REGISTER_COMPONENT(ClassName)                                         \
public:                                                                       \
    static constexpr Spark::ECS::ComponentTypeID ID = STRINGHASH(#ClassName); \
    static constexpr const char* Name = #ClassName;                           \
private:

#endif //SPARK_ECS_UTILITY_H
