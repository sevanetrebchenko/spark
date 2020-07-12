
#ifndef DATASTRUCTURES_BASE_COMPONENT_SYSTEM_INTERFACE_H
#define DATASTRUCTURES_BASE_COMPONENT_SYSTEM_INTERFACE_H

namespace ECS::Systems {
    // Interface for making ComponentSystems polymorphic at runtime + storage.
    class BaseComponentSystemInterface {
        public:
            virtual void Initialize() = 0;
            virtual void Update(float dt) = 0;
            virtual void Shutdown() = 0;
    };
}

#endif //DATASTRUCTURES_BASE_COMPONENT_SYSTEM_INTERFACE_H
