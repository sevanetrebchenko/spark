
#ifndef BASE_COMPONENT_SYSTEM_TPP
#define BASE_COMPONENT_SYSTEM_TPP

#include "../Components/base_component.h"

namespace ECS::Components {
    template <class... ComponentType>
    inline ComponentSystem<ComponentType...>::ComponentSystem() {
        static_assert((std::is_base_of<ECS::Components::BaseComponent, ComponentType>::value && ...), "Invalid template parameter provided to base ComponentSystem - component types must derive from ECS::Components::BaseComponent.");

        // Defer initialization to second stage.
    }

    template <class... ComponentType>
    inline void ComponentSystem<ComponentType...>::Initialize() {

    }

    template <class... ComponentType>
    inline ComponentSystem<ComponentType...>::~ComponentSystem() {
        _filteredEntities.clear();
    }
}

#endif // BASE_COMPONENT_SYSTEM_TPP
