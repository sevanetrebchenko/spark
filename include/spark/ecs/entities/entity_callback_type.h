
#ifndef SPARK_ENTITY_CALLBACK_TYPE_H
#define SPARK_ENTITY_CALLBACK_TYPE_H

namespace Spark {
    namespace ECS {
        namespace Entities {

            // Component system callback function types.
            enum class CallbackType {
                ENTITY_CREATE,
                ENTITY_DELETE,
                COMPONENT_ADD,
                COMPONENT_REMOVE
            };

        }
    }
}

#endif // SPARK_ENTITY_CALLBACK_TYPE_H
