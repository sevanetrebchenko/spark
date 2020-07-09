
#include "../include/entity_manager.h"

namespace ECS::Entities {
    // Two stage initialization: initializes resources associated with the Entity Manager, including support for logging
    // and initializing entity component storage.
    void EntityManager::Initialize() {
        if (!_data) {
            _data = new EntityManagerData();
        }
    }

    // Cleans up any resources associated with the Entity Manager.
    EntityManager::~EntityManager() {
        delete _data;
    }

    // Create an entity. Throws error if the provided entity name matches any of the the build-in component type names
    // or any pre-existing entity names. Automatically notifies all fully registered component systems that a new entity has been created.
    void EntityManager::CreateEntity(std::string name) {
        _data->CreateEntity(std::move(name));
    }

    // Destroy an entity from the Entity Manager with the provided entity ID, given that it exists. Automatically notifies
    // all fully registered component systems that an entity has been deleted.
    void EntityManager::DestroyEntity(EntityID ID) {
        _data->DestroyEntity(ID);
    }

    // Destroy an entity from the Entity Manager with the provided entity name, given that it exists. Automatically notifies
    // all fully registered component systems that an entity has been deleted.
    void EntityManager::DestroyEntity(std::string name) {
        _data->DestroyEntity(std::move(name));
    }

    // Retrieve the list of components that are attached to an entity with the provided ID, given that such an entity
    // exists in the Entity Manager.
    const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& EntityManager::GetComponents(EntityID ID) const {
        return _data->GetComponents(ID);
    }

    // Retrieve the list of components that are attached to an entity with the provided name, given that such an entity
    // exists in the Entity Manager.
    const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& EntityManager::GetComponents(std::string name) const {
        return _data->GetComponents(std::move(name));
    }
}
