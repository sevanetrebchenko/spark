
#ifndef ENTITY_MANAGER_TPP
#define ENTITY_MANAGER_TPP

#include <functional>                                   // std::function
#include "../../World/all_components.h"                 // ALL_COMPONENTS
#include "../Components/component_manager_collection.h" // ComponentManagerCollection

namespace ECS::Entities {
    //------------------------------------------------------------------------------------------------------------------
    // ENTITY MANAGER DATA
    //------------------------------------------------------------------------------------------------------------------
    class EntityManager::EntityManagerData {
        public:
            /**
             * Constructor.
             */
            EntityManagerData();

            /**
             * Cleans up any resources associated with the Entity Manager helper class.
             */
            ~EntityManagerData();

            /**
             * Create an entity. Throws error if the provided entity name matches any of the the build-in component type
             * names or any pre-existing entity names. Automatically notifies all fully registered component systems that
             * a new entity has been created.
             * @param name - Unique name for the entity to have.
             */
            void CreateEntity(std::string name);

            /**
             * Destroy an entity from the Entity Manager with the provided entity ID, given that it exists.
             * @param ID - ID of the entity to delete.
             */
            void DestroyEntity(EntityID ID);

            /**
             * Destroy an entity from the Entity Manager with the provided entity name, given that it exists.
             * @param name - Name of the entity to delete.
             */
            void DestroyEntity(std::string name);

            /**
             * Function to capture a class member function to convert it to a standard callback function callable from
             * within the Entity Manager as a response to changes to entities or their components.
             * @tparam Class             - Class type which holds the member function.
             * @tparam ReturnType        - Return type of the member function.
             * @tparam FunctionArguments - Parameter types of the member function.
             * @param  classInstance     - Class 'this' pointer.
             * @param  memberFunction    - Pointer to the member function to call.
             */
            template <class Class, typename ReturnType, typename ...FunctionArguments>
            void RegisterCallback(CallbackType, Class* classInstance, ReturnType(Class::*memberFunction)(FunctionArguments...));

            /**
             * Retrieve the list of components that are attached to an entity with the provided ID, given that such an
             * entity exists in the Entity Manager.
             * @param ID - ID of the entity to get the components of.
             * @return List of attached components to the entity at the provided ID.
             */
            const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& GetComponents(EntityID ID) const;

            /**
             * Retrieve the list of components that are attached to an entity with the provided name, given that such an
             * entity exists in the Entity Manager.
             * @param name - Name of the entity to get the components of.
             * @return List of attached components to the entity at the provided name.
             */
            const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& GetComponents(std::string name) const;

            /**
             * Attach a component to an entity at the provided ID, given that it exists and the entity doesn't already
             * have a component of that type attached to it. Automatically notifies all fully registered component systems
             * that a component has been added to this entity.
             * @tparam ComponentType - Type of component to attach.
             * @param  ID            - ID of the entity to attach component to.
             */
            template <class ComponentType>
            void AddComponent(EntityID ID);

            /**
             * Attach a component to an entity at the provided name, given that it exists and the entity doesn't already
             * have a component of that type attached to it. Automatically notifies all fully registered component systems
             * that a component has been added to this entity.
             * @tparam ComponentType - Type of component to attach.
             * @param  name          - Name of the entity to attach component to.
             */
            template <class ComponentType>
            void AddComponent(std::string name);

            /**
             * Remove a component from an entity at the provided ID, given that the entity exists in the Entity Manager.
             * It is valid behavior to delete a component from an entity that does not manage a component of that type.
             * Automatically notifies all fully registered component systems that a component has been removed from this
             * entity.
             * @tparam ComponentType - Type of component to attach.
             * @param  ID            - ID of the entity from which to remove the component.
             */
            template <class ComponentType>
            void DeleteComponent(EntityID ID);

            /**
             * Remove a component from an entity at the provided nae, given that the entity exists in the Entity Manager.
             * It is valid behavior to delete a component from an entity that does not manage a component of that type.
             * Automatically notifies all fully registered component systems that a component has been removed from this
             * entity.
             * @tparam ComponentType - Type of component to attach.
             * @param  name          - Name of the entity from which to remove the component.
             */
            template <class ComponentType>
            void DeleteComponent(std::string name);

        private:
            /**
             * Check the name of the given entity name against the names of all of the built-in components to ensure the
             * names do not overlap. Names are all converted to lowercase before comparison.
             * @tparam ComponentTypes - All the built-in component types.
             * @param name            - Entity name to be compared.
             * @return On
             */
            template <class ...ComponentTypes>
            bool CheckEntityName(const std::string& name) const;

            /**
             * Convert a given string to lowercase.
             * @param string - String to convert to lowercase.
             */
            void ConvertToLowercase(std::string& string) const;

            /**
             * Captures the data type of both the class and member function and constructs a lambda function to be able
             * to call the member function from outside of the class.
             * @tparam Class             - Class type which holds the member function.
             * @tparam ReturnType        - Return type of the member function.
             * @tparam FunctionArguments - Parameter types of the member function.
             * @param  classInstance     - Class 'this' pointer.
             * @param  memberFunction    - Pointer to the member function to call.
             * @return Standalone lambda function to ues to call the class member function.
             */
            template <class Class, typename ReturnType, typename ...FunctionArguments>
            auto CreateMemberFunction(Class* classInstance, ReturnType(Class::*memberFunction)(FunctionArguments...));

            /**
             * Convert an instance of an EntityManager::CallbackType object to a string to use in debug messages.
             * @param callbackType - Type of callback function to convert.
             * @return String representation of the callback function type.
             */
            const char* CallbackTypeToString(EntityManager::CallbackType callbackType);

            // Typedef for entity component list readability.
            typedef std::unordered_map<ComponentTypeID, Components::BaseComponent*> entityComponentList;

            std::unordered_map<EntityID, entityComponentList> _entityComponents; // List of components attached to entities.
            std::unordered_map<EntityID, std::string> _entityNames;              // Lsit of entity names.

            // Typedefs for component system callback functions.
            typedef std::function<void(EntityID)> OnEntityCreate;
            typedef std::function<void(EntityID)> OnEntityDestroy;
            typedef std::function<void(EntityID)> OnEntityComponentAdd;
            typedef std::function<void(EntityID)> OnEntityComponentRemove;

            std::vector<OnEntityCreate> _entityCreateCallbackFunctions;             // Callback functions for when entities get created.
            std::vector<OnEntityDestroy> _entityDestroyCallbackFunctions;           // Callback functions for when entities get created.
            std::vector<OnEntityComponentAdd> _componentAddCallbackFunctions;       // Callback functions for when entities get a component attached.
            std::vector<OnEntityComponentRemove> _componentRemoveCallbackFunctions; // Callback functions for when entities get a component removed.

            UtilityBox::Logger::LoggingSystem _loggingSystem { "Entity Manager" }; // Entity manager logging system.
    };

    // Function to capture a class member function to convert it to a standard callback function callable from within
    // the Entity Manager as a response to changes to entities or their components.
    template<class Class, typename ReturnType, typename... FunctionArguments>
    inline void EntityManager::EntityManagerData::RegisterCallback(EntityManager::CallbackType callbackType, Class *classInstance, ReturnType (Class::*memberFunction)(FunctionArguments...)) {
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
    inline void EntityManager::EntityManagerData::AddComponent(EntityID ID) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function AddComponent with entity ID: %i.", ID);

        // Ensure the entity with the given ID exists.
        auto entityIterator = _entityComponents.find(ID);
        if (entityIterator == _entityComponents.cend()) {
            message.Supply("Entity location was not found in entity manager - no entity exists at ID: %i. Error message issued.", ID);
            _loggingSystem.Log(message);

            UtilityBox::Logger::LogMessage errorMessage { UtilityBox::Logger::LogMessageSeverity::SEVERE };
            errorMessage.Supply("In function AddComponent: Entity ID provided is invalid - no entity exists at ID: %i.", ID);
            _loggingSystem.Log(errorMessage);

            throw std::invalid_argument("In function AddComponent: Entity ID provided is invalid - no entity exists at the provided ID.");
        }

        // Ensure the entity doesn't already have the component attached.
        auto entityComponentIterator = entityIterator->second.find(ComponentType::ID);
        if (entityComponentIterator != _entityComponents.end()) {
            message.Supply("Entities cannot hold multiple components of the same type. Entity at ID: %i already has a component with type: '%s'. Error message issued.", ID, ComponentType::Name);
            _loggingSystem.Log(message);

            UtilityBox::Logger::LogMessage errorMessage { UtilityBox::Logger::LogMessageSeverity::SEVERE };
            errorMessage.Supply("In function AddComponent: Entities cannot hold multiple components of the same type. Entity at ID: %i already has a component with type: '%s'.", ID, ComponentType::Name);
            _loggingSystem.Log(errorMessage);

            throw std::invalid_argument("In function AddComponent: Entities cannot hold multiple components of the same type. Entity at the provided ID already has an instance of the desired component type.");
        }

        // Retrieve entity manager from the engine world, construct a component, and append it to the entity component list of the given entity.
        Components::ComponentManager<ComponentType>* componentManager = Components::ComponentManagerCollection<ALL_COMPONENTS>::GetInstance()->GetComponentManager<ComponentType>();
        _entityComponents.at(ID).insert(ComponentType::ID, componentManager->CreateComponent());

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
    inline void EntityManager::EntityManagerData::AddComponent(std::string name) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function AddComponent with entity name: '%s.'", name.c_str());

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
    inline void EntityManager::EntityManagerData::DeleteComponent(EntityID ID) {
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
            message.Supply("Entity location was not found in entity manager - no entity exists at ID: %i. Warning message issued.", ID);
            _loggingSystem.Log(message);

            UtilityBox::Logger::LogMessage warningMessage { UtilityBox::Logger::LogMessageSeverity::WARNING };
            message.Supply("Entity at given entity ID to delete component of is invalid - no entity exists at ID: %i.", ID);
            _loggingSystem.Log(message);
        }
    }

    // Remove a component from an entity at the provided name, given that the entity exists in the Entity Manager. It is
    // valid behavior to delete a component from an entity that does not manage a component of that type. Automatically
    // notifies all fully registered component systems that a component has been removed from this entity.
    template<class ComponentType>
    inline void EntityManager::EntityManagerData::DeleteComponent(std::string name) {
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
    inline bool EntityManager::EntityManagerData::CheckEntityName(const std::string& entityName) const {
        // Return true if strcasecmp returns 0 (names are equal). Strcasecmp is used to ensure lowercase letters for both
        // entity names and component names are both lowercase.
        return (!(strcasecmp(entityName.c_str(), ComponentTypes::Name)) || ...);
    }

    // Captures the data type of both the class and member function and constructs a lambda function to be able to call
    // the member function from outside of the class.
    template<class Class, typename ReturnType, typename... Args>
    inline auto EntityManager::EntityManagerData::CreateMemberFunction(Class *classInstance, ReturnType (Class::*memberFunction)(Args...)) {
        return [classInstance, memberFunction](Args... additionalArguments) {
            // 'this' argument gets passed implicitly into member functions. Mimic this same behavior explicitly
            // to be able to call the class member function correctly.
            (ReturnType)(std::mem_fn(memberFunction)(classInstance, additionalArguments...));
        };
    }


    //------------------------------------------------------------------------------------------------------------------
    // ENTITY MANAGER
    //------------------------------------------------------------------------------------------------------------------

    // Function to capture a class member function to convert it to a standard callback function callable from within
    // the Entity Manager as a response to changes to entities or their components.
    template<class Class, typename ReturnType, typename... FunctionArguments>
    inline void EntityManager::RegisterCallback(EntityManager::CallbackType callbackType, Class *classInstance, ReturnType(Class::* memberFunction)(FunctionArguments...)) {
        _data->RegisterCallback<Class, ReturnType, FunctionArguments...>(callbackType, classInstance, memberFunction);
    }

    // Attach a component to an entity at the provided ID, given that it exists and the entity doesn't already have a
    // component of that type attached to it. Automatically notifies all fully registered component systems that a
    // component has been added to this entity.
    template<class ComponentType>
    inline void EntityManager::AddComponent(EntityID ID) {
        // Ensure given component type is valid.
        static_assert(std::is_base_of<Components::BaseComponent, ComponentType>::value, "In function AddComponent: Invalid component type provided.");
        _data->AddComponent<ComponentType>(ID);
    }

    // Attach a component to an entity at the provided name, given that it exists and the entity doesn't already have a
    // component of that type attached to it. Automatically notifies all fully registered component systems that a
    // component has been added to this entity.
    template<class ComponentType>
    inline void EntityManager::AddComponent(std::string name) {
        // Ensure given component type is valid.
        static_assert(std::is_base_of<Components::BaseComponent, ComponentType>::value, "In function AddComponent: Invalid component type provided.");
        _data->AddComponent<ComponentType>(std::move(name));
    }

    // Remove a component from an entity at the provided ID, given that the entity exists in the Entity Manager. It is
    // valid behavior to delete a component from an entity that does not manage a component of that type. Automatically
    // notifies all fully registered component systems that a component has been removed from this entity.
    template<class ComponentType>
    inline void EntityManager::DeleteComponent(EntityID ID) {
        // Ensure given component type is valid.
        static_assert(std::is_base_of<Components::BaseComponent, ComponentType>::value, "In function DeleteComponent: Invalid component type provided.");
        _data->DeleteComponent<ComponentType>(ID);
    }

    // Remove a component from an entity at the provided name, given that the entity exists in the Entity Manager. It is
    // valid behavior to delete a component from an entity that does not manage a component of that type. Automatically
    // notifies all fully registered component systems that a component has been removed from this entity.
    template<class ComponentType>
    inline void EntityManager::DeleteComponent(std::string name) {
        // Ensure given component type is valid.
        static_assert(std::is_base_of<Components::BaseComponent, ComponentType>::value, "In function DeleteComponent: Invalid component type provided.");
        _data->DeleteComponent<ComponentType>(std::move(name));
    }
}

#endif // ENTITY_MANAGER_TPP
