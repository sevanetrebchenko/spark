
#ifndef DATASTRUCTURES_ENTITY_CALLBACK_TYPE_H
#define DATASTRUCTURES_ENTITY_CALLBACK_TYPE_H

namespace ECS::Entities {
    // Component system callback function types.
    enum class CallbackType {
        ENTITY_CREATE,
        ENTITY_DELETE,
        COMPONENT_ADD,
        COMPONENT_REMOVE
    };
}

#endif //DATASTRUCTURES_ENTITY_CALLBACK_TYPE_H
