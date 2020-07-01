
#ifndef ENTITY_MANAGER_TPP
#define ENTITY_MANAGER_TPP

namespace ECS::Entities {
    template<class ComponentType>
    inline ComponentType* EntityManager::GetComponent(EntityID ID) {
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
    inline void EntityManager::AddComponent(EntityID ID) {
        // Ensure given component type is valid.
        static_assert(std::is_base_of<ECS::Components::BaseComponent, ComponentType>::value, "Invalid component type provided to EntityManager.");

        // Ensure the entity with the given ID exists in the world.
        _entityList::const_iterator iterID = _entities.find(ID);
        if (iterID == _entities.cend()) {
            return; //todo
        }

//            _entities.at(ID).insert(ComponentType::ID, // get component here);

        // Call all callback functions to notify systems.
        for (auto &componentAddCallbackFunction : _componentAddCallbackFunctions) {
            (*componentAddCallbackFunction)(ID);
        }
    }

    template<class ComponentType>
    inline void EntityManager::DeleteComponent(EntityID ID) {
        // Ensure given component type is valid.
        static_assert(std::is_base_of<ECS::Components::BaseComponent, ComponentType>::value, "Invalid component type provided to EntityManager.");

        // Ensure the entity with the given ID exists in the world.
        _entityList::const_iterator iterID = _entities.find(ID);
        if (iterID == _entities.cend()) {
            return; //todo
        }

        // Component found - delete.
//            _entities.at(ID).insert(ComponentType::ID, // get component here);

        // Call all callback functions to notify systems.
        for (auto &componentRemoveCallbackFunction : _componentRemoveCallbackFunctions) {
            (*componentRemoveCallbackFunction)(ID);
        }
    }
}

#endif // ENTITY_MANAGER_TPP
