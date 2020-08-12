
#ifndef SPARK_ENTITY_MANAGER_DATA_TPP
#define SPARK_ENTITY_MANAGER_DATA_TPP

#include <ecs/all_components.h>                         // ALL_COMPONENTS
#include <utilitybox/tools/compile_time_hash.h>          // STRINGHASH
#include <ecs/components/component_manager_collection.h> // ComponentManagerCollection

namespace Spark::ECS::Entities {
    // Function to capture a class member function to convert it to a standard callback function callable from within
    // the Entity Manager as a response to changes to entities or their components.
    template<class Class, typename ReturnType, typename... FunctionArguments>
    inline void EntityManagerData::RegisterCallback(CallbackType callbackType, Class *classInstance, ReturnType (Class::*memberFunction)(FunctionArguments...)) {
        UtilityBox::Logger::LogMessage message {};
        const char* callbackTypeToString = CallbackTypeToString(callbackType);
        message.Supply("Entering function RegisterCallback with component system callback type: '%s.'", callbackTypeToString);
        message.Supply("Constructing callable class member callback function.");

        // Construct lambda expression capturing class 'this' pointer and member function to call.
        std::function<void(EntityID)> function = CreateMemberFunction<Class, ReturnType, EntityID>(classInstance, memberFunction);

        // Append the function to the correct location.
        switch (callbackType) {
            case CallbackType::ENTITY_CREATE:
                _entityCreateCallbackFunctions.emplace_back(std::move(function));
                break;
            case CallbackType::ENTITY_DELETE:
                _entityDestroyCallbackFunctions.emplace_back(std::move(function));
                break;
            case CallbackType::COMPONENT_ADD:
                _componentAddCallbackFunctions.emplace_back(std::move(function));
                break;
            case CallbackType::COMPONENT_REMOVE:
                _componentRemoveCallbackFunctions.emplace_back(std::move(function));
                break;
        }

        message.Supply("Successfully enabled tracking for member callback function of type: '%s'.", callbackTypeToString);
        _loggingSystem.Log(message);
    }

    // Attach a component to an entity at the provided ID, given that it exists and the entity doesn't already have a
    // component of that type attached to it. Automatically notifies all fully registered component systems that a
    // component has been added to this entity.
    template<class ComponentType>
    inline void EntityManagerData::AddComponent(EntityID ID) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function AddComponent with entity ID: %i.", ID);

        // Ensure the entity with the given ID exists.
        auto entityIterator = _entityComponents.find(ID);
        if (entityIterator == _entityComponents.end()) {
            message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::SEVERE);
            message.Supply("Entity location was not found in entity manager - no entity exists at ID: %i.", ID);
            _loggingSystem.Log(message);

            throw std::invalid_argument("In function AddComponent: Entity ID provided is invalid - no entity exists at the provided ID.");
        }

        // Ensure the entity doesn't already have the component attached.
        auto entityComponentIterator = entityIterator->second.find(ComponentType::ID);
        if (entityComponentIterator != entityIterator->second.end()) {
            message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::SEVERE);
            message.Supply("entities cannot hold multiple components of the same type. Entity at ID: %i already has a component with type: '%s'.", ID, ComponentType::Name);
            _loggingSystem.Log(message);

            throw std::invalid_argument("In function AddComponent: entities cannot hold multiple components of the same type. Entity at the provided ID already has an instance of the desired component type.");
        }

        // Retrieve entity manager from the engine world, construct a component, and append it to the entity component list of the given entity.
        Components::ComponentManager<ComponentType>* componentManager = Components::ComponentManagerCollection<ALL_COMPONENTS>::GetInstance()->GetComponentManager<ComponentType>();
        _entityComponents.at(ID)[ComponentType::ID] = componentManager->CreateComponent();

        message.Supply("Successfully added a default constructed component of type: '%s' to entity.", ComponentType::Name);

        // Call all callback functions to notify systems.
        message.Supply("Component system 'OnEntityComponentAdd' callback functions called.");
        _loggingSystem.Log(message);

        for (auto &componentAddCallbackFunction : _componentAddCallbackFunctions) {
            componentAddCallbackFunction(ID);
        }
    }

    // Attach a component to an entity at the provided name, given that it exists and the entity doesn't already have a
    // component of that type attached to it. Automatically notifies all fully registered component systems that a
    // component has been added to this entity.
    template<class ComponentType>
    inline void EntityManagerData::AddComponent(std::string name) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function AddComponent.  with entity name: '%s' and component type: '%s'.", name.c_str(), ComponentType::Name);

        ConvertToLowercase(name);
        message.Supply("Entity name converted to lowercase: '%s.'", name.c_str());

        // Hand off responsibility to overloaded function.
        message.Supply("Calling function AddComponent with hashed entity ID.");
        _loggingSystem.Log(message);

        AddComponent<ComponentType>(STRINGHASH(name.c_str()));
    }

    // Remove a component from an entity at the provided ID, given that the entity exists in the Entity Manager. It is
    // valid behavior to delete a component from an entity that does not manage a component of that type. Automatically
    // notifies all fully registered component systems that a component has been removed from this entity.
    template<class ComponentType>
    inline void EntityManagerData::DeleteComponent(EntityID ID) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function DeleteComponent with entity ID: %i.", ID);

        // Ensure the entity with the given ID exists.
        const auto entityIterator = _entityComponents.find(ID);
        if (entityIterator != _entityComponents.cend()) {
            message.Supply("Found entity location within entity manager.");

            // Retrieve entity manager from the engine world.
            Components::ComponentManager<ComponentType>* componentManager = Components::ComponentManagerCollection<ALL_COMPONENTS>::GetInstance()->GetComponentManager<ComponentType>();

            // Delete memory associated with component.
            message.Supply("Returning component of type: '%s' back to associated Component Manager.", ComponentType::Name);
            componentManager->DeleteComponent(dynamic_cast<ComponentType*>(_entityComponents.at(ID).at(ComponentType::ID)));

            message.Supply("Removing component from entity component list.");
            _entityComponents.at(ID).erase(ComponentType::ID);

            // Call all callback functions to notify systems.
            message.Supply("Component system 'OnEntityComponentRemove' callback functions called.");
            _loggingSystem.Log(message);

            for (auto &componentRemoveCallbackFunction : _componentRemoveCallbackFunctions) {
                componentRemoveCallbackFunction(ID);
            }
        }
        else {
            message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::WARNING);
            message.Supply("Entity location was not found in entity manager - no entity exists at ID: %i.", ID);
            _loggingSystem.Log(message);
        }
    }

    // Remove a component from an entity at the provided name, given that the entity exists in the Entity Manager. It is
    // valid behavior to delete a component from an entity that does not manage a component of that type. Automatically
    // notifies all fully registered component systems that a component has been removed from this entity.
    template<class ComponentType>
    inline void EntityManagerData::DeleteComponent(std::string name) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function DeleteComponent with entity name: '%s.'", name.c_str());

        ConvertToLowercase(name);
        message.Supply("Entity name converted to lowercase: '%s.'", name.c_str());

        // Hand off responsibility to overloaded function.
        message.Supply("Calling function DeleteComponent with hashed entity ID.");
        _loggingSystem.Log(message);

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

    // Captures the data type of both the class and member function and constructs a lambda function to be able to call
    // the member function from outside of the class.
    template<class Class, typename ReturnType, typename... Args>
    inline auto EntityManagerData::CreateMemberFunction(Class *classInstance, ReturnType (Class::*memberFunction)(Args...)) {
        return [classInstance, memberFunction](Args... additionalArguments) {
            // 'this' argument gets passed implicitly into member functions. Mimic this same behavior explicitly
            // to be able to call the class member function correctly.
            (ReturnType)(std::mem_fn(memberFunction)(classInstance, additionalArguments...));
        };
    }
}

#endif // SPARK_ENTITY_MANAGER_DATA_TPP
