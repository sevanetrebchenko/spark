//
// Created by seva on 6/23/20.
//

#include "base_component_system.h"
#include "../Components/base_component.h"

namespace ECS::Components {
    template <class... ComponentType>
    ComponentSystem<ComponentType...>::ComponentSystem() {
        static_assert((std::is_base_of<ECS::Components::BaseComponent, ComponentType>::value && ...), "Invalid component type provided to ComponentSystem.");

        // Defer initialization to second stage.
    }

    template <class... ComponentType>
    void ComponentSystem<ComponentType...>::Initialize() {

    }

    template <class... ComponentType>
    ComponentSystem<ComponentType...>::~ComponentSystem() {
        _filteredEntities.clear();
    }
}
