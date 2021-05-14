
#ifndef SPARK_COMPONENT_MANAGER_INTERFACE_H
#define SPARK_COMPONENT_MANAGER_INTERFACE_H

namespace Spark {
    namespace ECS {
        namespace Components {

            // Virtual abstract interface so that component managers can be polymorphic at runtime.
            class ComponentManagerInterface {
                public:
                    virtual ~ComponentManagerInterface() = default;
            };

        }
    }
}

#endif // SPARK_COMPONENT_MANAGER_INTERFACE_H
