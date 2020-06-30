
#include "entity_manager.h"
#include <algorithm>

namespace ECS::Entities {
    EntityManager::EntityManager() {
    }

    void EntityManager::CreateEntity(const char *name) {
    }

    const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& EntityManager::GetComponents(EntityID ID) {
        return _entities.at(ID);
    }
}


