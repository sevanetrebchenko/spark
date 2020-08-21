
#ifndef SPARK_ENTITY_MANAGER_DATA_TPP
#define SPARK_ENTITY_MANAGER_DATA_TPP

#include <ecs/all_components.h>                         // ALL_COMPONENTS
#include <utilitybox/tools/compile_time_hash.h>          // STRINGHASH
#include <ecs/components/component_manager_collection.h> // ComponentManagerCollection
#include <utilitybox/tools/utility_functions.h>

namespace Spark::ECS::Entities {
    // Attach a component to an entity at the provided ID, given that it exists and the entity doesn't already have a
    // component of that type attached to it. Automatically notifies all fully registered component systems that a
    // component has been added to this entity.
    template<class ComponentType>
    inline void EntityManagerData::AddComponent(EntityID ID) {
        LogDebug("Entering function AddComponent with entity ID: %i.", ID);

        // Ensure the entity with the given ID exists.
        auto entityIterator = _entityComponents.find(ID);
        if (entityIterator == _entityComponents.end()) {
            LogError("Entity location was not found in entity manager - no entity exists at ID: %i.", ID);
            throw std::invalid_argument("In function AddComponent: Entity ID provided is invalid - no entity exists at the provided ID.");
        }

        // Ensure the entity doesn't already have the component attached.
        auto entityComponentIterator = entityIterator->second.find(ComponentType::ID);
        if (entityComponentIterator != entityIterator->second.end()) {
            LogError("entities cannot hold multiple components of the same type. Entity at ID: %i already has a component with type: '%s'.", ID, ComponentType::Name);
            throw std::invalid_argument("In function AddComponent: entities cannot hold multiple components of the same type. Entity at the provided ID already has an instance of the desired component type.");
        }

        // Retrieve entity manager from the engine world, construct a component, and append it to the entity component list of the given entity.
        Components::ComponentManager<ComponentType>* componentManager = Components::ComponentManagerCollection<ALL_COMPONENTS>::GetInstance()->GetComponentManager<ComponentType>();
        _entityComponents.at(ID)[ComponentType::ID] = componentManager->CreateComponent();

        LogDebug("Successfully added a default constructed component of type: '%s' to entity.", ComponentType::Name);

        // Call all callback functions to notify systems.
        LogDebug("Component system 'OnEntityComponentAdd' callback functions called.");

        for (auto &componentAddCallbackFunction : _componentAddCallbackFunctions) {
            componentAddCallbackFunction(ID);
        }
    }

    // Attach a component to an entity at the provided name, given that it exists and the entity doesn't already have a
    // component of that type attached to it. Automatically notifies all fully registered component systems that a
    // component has been added to this entity.
    template<class ComponentType>
    inline void EntityManagerData::AddComponent(std::string name) {
        LogDebug("Entering function AddComponent.  with entity name: '%s' and component type: '%s'.", name.c_str(), ComponentType::Name);

        ConvertToLowercase(name);
        LogDebug("Entity name converted to lowercase: '%s.'", name.c_str());

        // Hand off responsibility to overloaded function.
        LogDebug("Calling function AddComponent with hashed entity ID.");
        AddComponent<ComponentType>(STRINGHASH(name.c_str()));
    }

    // Remove a component from an entity at the provided ID, given that the entity exists in the Entity Manager. It is
    // valid behavior to delete a component from an entity that does not manage a component of that type. Automatically
    // notifies all fully registered component systems that a component has been removed from this entity.
    template<class ComponentType>
    inline void EntityManagerData::DeleteComponent(EntityID ID) {
        LogDebug("Entering function DeleteComponent with entity ID: %i.", ID);

        // Ensure the entity with the given ID exists.
        const auto entityIterator = _entityComponents.find(ID);
        if (entityIterator != _entityComponents.cend()) {
            LogDebug("Found entity location within entity manager.");

            // Retrieve entity manager from the engine world.
            Components::ComponentManager<ComponentType>* componentManager = Components::ComponentManagerCollection<ALL_COMPONENTS>::GetInstance()->GetComponentManager<ComponentType>();

            // Delete memory associated with component.
            LogDebug("Returning component of type: '%s' back to associated Component Manager.", ComponentType::Name);
            componentManager->DeleteComponent(dynamic_cast<ComponentType*>(_entityComponents.at(ID).at(ComponentType::ID)));

            LogDebug("Removing component from entity component list.");
            _entityComponents.at(ID).erase(ComponentType::ID);

            // Call all callback functions to notify systems.
            LogDebug("Component system 'OnEntityComponentRemove' callback functions called.");

            for (auto &componentRemoveCallbackFunction : _componentRemoveCallbackFunctions) {
                componentRemoveCallbackFunction(ID);
            }
        }
        else {
            LogWarning("Entity location was not found in entity manager - no entity exists at ID: %i.", ID);
        }
    }

    // Remove a component from an entity at the provided name, given that the entity exists in the Entity Manager. It is
    // valid behavior to delete a component from an entity that does not manage a component of that type. Automatically
    // notifies all fully registered component systems that a component has been removed from this entity.
    template<class ComponentType>
    inline void EntityManagerData::DeleteComponent(std::string name) {
        LogDebug("Entering function DeleteComponent with entity name: '%s.'", name.c_str());

        ConvertToLowercase(name);
        LogDebug("Entity name converted to lowercase: '%s.'", name.c_str());

        // Hand off responsibility to overloaded function.
        LogDebug("Calling function DeleteComponent with hashed entity ID.");
        DeleteComponent<ComponentType>(STRINGHASH(name.c_str()));
    }

    // Check the name of the given entity name against the names of all of the built-in components to ensure the names
    // do not overlap. Names are all converted to lowercase before comparison.
    template<class... ComponentTypes>
    inline bool EntityManagerData::CheckEntityName(const std::string& entityName) const {
        // Return true if strcasecmp returns 0 (names are equal). Strcasecmp is used to ensure lowercase letters for both
        // entity names and component names are both lowercase.
        return (!(strcasecmp(entityName.c_str(), ComponentTypes::Name)) || ...);
    }

}

#endif // SPARK_ENTITY_MANAGER_DATA_TPP
