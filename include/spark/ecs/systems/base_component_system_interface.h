
#ifndef SPARK_BASE_COMPONENT_SYSTEM_INTERFACE_H
#define SPARK_BASE_COMPONENT_SYSTEM_INTERFACE_H

namespace Spark {
    namespace ECS {

        // Interface for making ComponentSystems polymorphic at runtime + making system storage easier.
        class IBaseComponentSystem {
            public:
                virtual void Initialize() = 0;
                virtual void Update(float dt) = 0;
                virtual void Shutdown() = 0;
        };

    }
}

#endif // SPARK_BASE_COMPONENT_SYSTEM_INTERFACE_H
