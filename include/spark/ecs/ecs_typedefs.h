
#ifndef SPARK_ECS_TYPEDEFS_H
#define SPARK_ECS_TYPEDEFS_H

#include <spark/core/rename.h>
#include <spark/ecs/components/types/base_component.h>

namespace Spark {
    namespace ECS {

        typedef unsigned ComponentTypeID; // Component type gets hashed into an unsigned value.
        typedef std::uint32_t EntityID;   // EntityID gets created by STRINGHASHing the entity name.
        typedef std::unordered_map<ComponentTypeID, BaseComponent*> EntityComponentMap;

    }
}

#endif // SPARK_ECS_TYPEDEFS_H
