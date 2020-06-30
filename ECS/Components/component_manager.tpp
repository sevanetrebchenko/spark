
#ifndef COMPONENT_MANAGER_TPP
#define COMPONENT_MANAGER_TPP

#include <type_traits>

namespace ECS::Components {
    template<class ComponentType>
    inline ComponentManager<ComponentType>::ComponentManager() {
        // Make sure all component managers only manage valid types.
        static_assert(std::is_base_of<ECS::Components::BaseComponent, ComponentType>::value, "Invalid template parameter provided to base ComponentManager - component type must derive from ECS::Components::BaseComponent.");
        // Defer initialization to second stage.
    }

    template <typename ComponentType>
    inline void ComponentManager<ComponentType>::Initialize() {
        _allocator.Initialize();
    }

    template<class ComponentType>
    inline ComponentManager<ComponentType>::~ComponentManager() {
    }

    template<class ComponentType>
    inline ComponentType* ComponentManager<ComponentType>::CreateComponent() {
        return static_cast<ComponentType*>(_allocator.RetrieveBlock());
    }

    template<class ComponentType>
    inline void ComponentManager<ComponentType>::DeleteComponent(ComponentType *component) {
        _allocator.ReturnBlock(component);
    }
}

#endif // COMPONENT_MANAGER_TPP
