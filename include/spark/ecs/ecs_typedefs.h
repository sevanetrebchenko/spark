
#ifndef SPARK_ECS_TYPEDEFS_H
#define SPARK_ECS_TYPEDEFS_H

#include <spark_pch.h> // std::uint32_t

namespace Spark {
    namespace ECS {

        typedef unsigned ComponentTypeID; // Component type gets hashed into an unsigned value.
        typedef std::uint32_t EntityID;   // EntityID gets created by STRINGHASHing the entity name.

    }
}

#endif // SPARK_ECS_TYPEDEFS_H
