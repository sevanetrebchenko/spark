
#ifndef DATASTRUCTURES_COMPONENT_MANAGER_INTERFACE_H
#define DATASTRUCTURES_COMPONENT_MANAGER_INTERFACE_H

namespace ECS::Components {
    // Virtual abstract interface so that component managers can be polymorphic at runtime.
    class ComponentManagerInterface {
        public:
            virtual ~ComponentManagerInterface() = default;
    };
}

#endif //DATASTRUCTURES_COMPONENT_MANAGER_INTERFACE_H
