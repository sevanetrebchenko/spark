
#ifndef DATASTRUCTURES_ENTITY_H
#define DATASTRUCTURES_ENTITY_H

#include "../ecs_typedefs.h"

namespace ECS::Entities {
    struct Entity {
        EntityID ID;
        const char* name;
    };
}



#endif //DATASTRUCTURES_ENTITY_H
