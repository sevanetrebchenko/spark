
#ifndef SPARK_COMPONENT_SYSTEM_COLLECTION_INTERFACE_H
#define SPARK_COMPONENT_SYSTEM_COLLECTION_INTERFACE_H

#include <spark/ecs/systems/base_component_system_interface.h>

namespace Spark {
    namespace ECS {
        namespace Systems {

            class IComponentSystemCollection {
                public:
                    virtual void AddSystem(IBaseComponentSystem* system);
                    virtual void RemoveSystem(IBaseComponentSystem* system);

                    virtual void Initialize() = 0;
                    virtual void Update(float dt) = 0;
                    virtual void Shutdown() = 0;
            };

        }
    }
}

#endif //SPARK_COMPONENT_SYSTEM_COLLECTION_INTERFACE_H
