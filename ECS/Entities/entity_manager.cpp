
#include "entity_manager.h"
#include <algorithm>

namespace ECS::Entities {
    EntityManager::EntityManager() {
    }

    void EntityManager::CreateEntity(const char *name) {
        // Generate unique ID.
    }

    const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& EntityManager::GetComponents(EntityID ID) {
        if (_entities.find(ID) == _entities.end()) {
            UtilityBox::Logger::LogMessage errorMessage {};
            errorMessage.Supply("Entity ID provided to EntityManager's GetComponents is invalid - no entity exists at ID: %i.", ID);
            throw std::out_of_range("Entity ID provided to EntityManager's GetComponents is invalid - no entity exists at the provided ID.");
        }

        return _entities.at(ID);
    }
}


