
#ifndef DATASTRUCTURES_COMPONENT_MANAGER_CPP
#define DATASTRUCTURES_COMPONENT_MANAGER_CPP
#include <type_traits>
#include "component_manager.h"

namespace ECS::Components {
    template<class ComponentType>
    ComponentManager<ComponentType>::ComponentManager() {
        // Make sure all component managers only manage valid types.
        static_assert(std::is_base_of<ECS::Components::BaseComponent, ComponentType>::value, "Invalid template parameter provided to base ComponentManager - component type must derive from ECS::Components::BaseComponent.");
        // Defer initialization to second stage.
    }

    template <typename ComponentType>
    void ComponentManager<ComponentType>::Initialize() {
        _allocator.Initialize();
    }

    template<class ComponentType>
    ComponentManager<ComponentType>::~ComponentManager() {
    }

    template<class ComponentType>
    ComponentType* ComponentManager<ComponentType>::CreateComponent() {
        return static_cast<ComponentType*>(_allocator.RetrieveBlock());
    }

    template<class ComponentType>
    void ComponentManager<ComponentType>::DeleteComponent(ComponentType *component) {
        _allocator.ReturnBlock(component);
    }
}

#endif // DATASTRUCTURES_COMPONENT_MANAGER_CPP
