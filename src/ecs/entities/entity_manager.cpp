
#include "ecs/entities/entity_manager.h" // EntityManager

namespace Spark::ECS::Entities {
    EntityManager::EntityManager() : _data(new EntityManagerData()) /* Throws on error, caught elsewhere. */ {
        // Nothing to do here.
    }

    // Cleans up any resources associated with the Entity Manager.
    EntityManager::~EntityManager() {
        delete _data;
    }

    // Create an entity. Throws error if the provided entity name matches any of the the bin-in component type names
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
