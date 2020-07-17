
#ifndef SPARK_BASE_COMPONENT_SYSTEM_INTERFACE_H
#define SPARK_BASE_COMPONENT_SYSTEM_INTERFACE_H

namespace Spark {
    namespace ECS {
        namespace Systems {

            // Interface for making ComponentSystems polymorphic at runtime + making system storage easier.
            class BaseComponentSystemInterface {
                public:
                    virtual void Initialize() = 0;
                    virtual void Update(float dt) = 0;
                    virtual void Shutdown() = 0;
            };

        } // namespace Systems
    } // namespace ECS
} // namespace Spark

#endif // SPARK_BASE_COMPONENT_SYSTEM_INTERFACE_H
