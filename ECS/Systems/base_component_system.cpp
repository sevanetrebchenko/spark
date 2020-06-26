//
// Created by seva on 6/23/20.
//

#include "base_component_system.h"
namespace ECS {
    namespace Components {
        template<class... ComponentType>
        ComponentSystem<ComponentType...>::ComponentSystem() {

            // Defer initialization to second stage.
        }

        template<class... ComponentType>
        void ComponentSystem<ComponentType...>::Initialize() {
        }

        template<class... ComponentType>
        ComponentSystem<ComponentType...>::~ComponentSystem() {
            _filteredEntities.clear();
        }
    }
}
