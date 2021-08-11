
#include "spark/ecs/entities/entity_manager.h"
#include "spark/utilitybox/logger/logger.h"
#include "spark/events/types/ecs_events.h"

namespace Spark::ECS {

    EntityManager::EntityManager() : entityIDCounter(0) {
    }

    void EntityManager::CreateEntity(const std::string& entityName) {
        std::string name = GetValidEntityName(entityName);
        if (name != entityName) {
            LogWarning("Entity name cannot match that of a built-in component or another entity. Entity name changed from '%s' to '%s'.", entityName.c_str(), name.c_str());
        }

        EntityID ID = GetNextEntityID();

        entityNames_[ID] = name;
        entityIDs_[entityName] = ID;

        // All entities start with a transform.
        // Index operator creates a new default constructed entry.
        entityComponents_[ID][Transform::ID] = static_cast<IComponent*>(Singleton<ComponentManagerCollection<COMPONENT_TYPES>>::GetInstance()->GetComponentManager<Transform>()->CreateComponent());
        Singleton<Events::EventHub>::GetInstance()->Dispatch(std::make_shared<Events::EntityCreatedEvent>(ID));
    }

    void EntityManager::DestroyEntity(EntityID ID) {
        auto entityNameIter = entityNames_.find(ID);
        auto entityComponentMapIter = entityComponents_.find(ID);

        bool bothExist = entityNameIter != entityNames_.end() && entityComponentMapIter != entityComponents_.end();
        bool bothDoNotExist = entityNameIter == entityNames_.end() && entityComponentMapIter == entityComponents_.end();

        SP_ASSERT(bothExist || bothDoNotExist, "Incorrectly configured entity in EntityManager. EntityID: %u", ID);

        if (bothDoNotExist) {
            LogWarning("Attemping to destroy entity that does not exist. Entity ID: %u", ID);
            return; // Do not destroy entity that does not exist.
        }

        // Return all components to corresponding managers.
        for (std::pair<const ComponentTypeID, IComponent*>& data : entityComponentMapIter->second) {
            DeleteComponentHelper<COMPONENT_TYPES>(data.first, data.second);
        }

        // Erase mappings.
        entityComponents_.erase(entityComponentMapIter);
        entityIDs_.erase(entityIDs_.find(entityNameIter->second)); // Guaranteed to exist.
        entityNames_.erase(entityNameIter);

        Singleton<Events::EventHub>::GetInstance()->Dispatch(std::make_shared<Events::EntityDestroyedEvent>(ID));
    }

    void EntityManager::DestroyEntity(const std::string& entityName) {
        DestroyEntity(GetEntityIDFromName(entityName));
    }

    EntityID EntityManager::GetEntityIDFromName(const std::string &entityName) const {
        auto entityNameIter = entityIDs_.find(entityName);
        if (entityNameIter != entityIDs_.end()) {
            return entityNameIter->second;
        }

        return INVALID_ID;
    }

    std::string EntityManager::GetValidEntityName(const std::string& entityName) const {
        unsigned long appendNum = 1;
        std::string current = entityName;

        while (EntityNameMatchesComponentName<COMPONENT_TYPES>(current) || EntityNameMatchesEntityName(current)) {
            current = entityName; // Reset for next append.
            current += std::to_string(appendNum++);
        }

        return current;
    }

    bool EntityManager::EntityNameMatchesEntityName(const std::string &entityName) const {
        if (entityNames_.empty()) {
            return false;
        }

        for (const auto& nameIter : entityNames_) {
            const std::string& name = nameIter.second;

            if (::Spark::Internal::StringCompare(name, entityName)) {
                // Current entity name matches an already existing name.
                return true;
            }
        }

        return false;
    }

    const EntityComponentMap* EntityManager::GetEntityComponentMap(const std::string &entityName) const {
        return GetEntityComponentMap(GetEntityIDFromName(entityName));
    }

    const EntityComponentMap* EntityManager::GetEntityComponentMap(EntityID entityID) const {
        auto componentMapIter = entityComponents_.find(entityID);
        if (componentMapIter == entityComponents_.end()) {
            return nullptr;
        }

        return &componentMapIter->second;
    }

    EntityID EntityManager::GetNextEntityID() {
        return entityIDCounter++;
    }

}
