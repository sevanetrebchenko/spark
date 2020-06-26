
#include "entity_manager.h"
#include <algorithm>

namespace ECS::Entities {
    EntityManager::EntityManager() {
    }

    void EntityManager::CreateEntity(const char *name) {
    }

    template<class ComponentType>
    ComponentType* EntityManager::GetComponent(EntityID ID) {
        // Ensure given component type is valid.
        static_assert(std::is_base_of<ECS::Components::BaseComponent, ComponentType>::value, "Invalid component type provided to EntityManager.");

        // Ensure the entity with the given ID exists in the world.
        _entityList::const_iterator iterID = _entities.find(ID);
        if (iterID == _entities.cend()) {
            return nullptr; // todo
        }

        // Find the key with the desired component.
        _entityComponentList& components = _entities.at(ID);
        _entityComponentList::const_iterator it = components.find(ComponentType::ID);

        // Desired component is not attached to this entity.
        if (it != components.end()) {
            return dynamic_cast<ComponentType*>(it->second);
        }
        else {
            return nullptr;
        }
    }

    template<class ComponentType>
    void EntityManager::AddComponent(EntityID ID) {
        // Ensure given component type is valid.
        static_assert(std::is_base_of<ECS::Components::BaseComponent, ComponentType>::value, "Invalid component type provided to EntityManager.");

        // Ensure the entity with the given ID exists in the world.
        _entityList::const_iterator iterID = _entities.find(ID);
        if (iterID == _entities.cend()) {
            return; //todo
        }

//            _entities.at(ID).insert(ComponentType::ID, // get component here);
    }

    template<class ComponentType>
    void EntityManager::DeleteComponent(EntityID ID) {
        // Ensure given component type is valid.
        static_assert(std::is_base_of<ECS::Components::BaseComponent, ComponentType>::value, "Invalid component type provided to EntityManager.");

        // Ensure the entity with the given ID exists in the world.
        _entityList::const_iterator iterID = _entities.find(ID);
        if (iterID == _entities.cend()) {
            return; //todo
        }

//            _entities.at(ID).insert(ComponentType::ID, // get component here);
    }

    const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& EntityManager::GetComponents(EntityID ID) {
        return _entities.at(ID);
    }
}


