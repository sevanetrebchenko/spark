
#ifndef ENTITY_MANAGER_TPP
#define ENTITY_MANAGER_TPP

#include "../../World/all_components.h" // ALL_COMPONENTS
#include "../../World/world.h"          // GetComponentManagerCollection
#include <functional>                   // std::function

namespace ECS::Entities {
    //------------------------------------------------------------------------------------------------------------------
    // ENTITY MANAGER DATA
    //------------------------------------------------------------------------------------------------------------------
    class EntityManager::EntityManagerData {
        public:
            /**
             * Constructor.
             */
            EntityManagerData() = default;

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

    // Cleans up any resources associated with the Entity Manager helper class.
    EntityManager::EntityManagerData::~EntityManagerData() {
        // Clear entity components.
        for (auto entityComponentList : _entityComponents) {
            entityComponentList.second.clear();
        }

        _entityComponents.clear();
        _entityNames.clear();

        // Clear callback functions.
        _entityCreateCallbackFunctions.clear();
        _entityDestroyCallbackFunctions.clear();
        _componentAddCallbackFunctions.clear();
        _componentRemoveCallbackFunctions.clear();
    }

    // Create an entity. Throws error if the provided entity name matches any of the the build-in component type names
    // or any pre-existing entity names. Automatically notifies all fully registered component systems that a new entity has been created.
    void EntityManager::EntityManagerData::CreateEntity(std::string name) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function CreateEntity with name: %s.", name.c_str());

        ConvertToLowercase(name);

        // Entity name cannot be a name of any component.
        if (CheckEntityName<ALL_COMPONENTS>(name)) {
            UtilityBox::Logger::LogMessage errorMessage { UtilityBox::Logger::LogMessageSeverity::SEVERE };
            errorMessage.Supply("In function CreateEntity: Provided entity name: '%s' cannot match a built-in component name.", name.c_str());
            _loggingSystem.Log(errorMessage);

            throw std::invalid_argument("In function CreateEntity: Provided entity name cannot match a built-in component name.");
        }

        EntityID hashedID = STRINGHASH(name.c_str());

        // Newly created entity cannot have a matching ID to an already existing entity.
        if (_entityComponents.find(hashedID) != _entityComponents.end()) {
            UtilityBox::Logger::LogMessage errorMessage { UtilityBox::Logger::LogMessageSeverity::SEVERE };
            errorMessage.Supply("In function CreateEntity: Provided entity name: '%s' matches an already existing entity name.", name.c_str());
            _loggingSystem.Log(errorMessage);

            throw std::invalid_argument("In function CreateEntity: Provided entity name matches an already existing entity name.");
        }

        // Index operator creates a new default constructed entry.
        _entityComponents[hashedID];
    }

    // Destroy an entity from the Entity Manager with the provided entity ID, given that it exists. Automatically notifies
    // all fully registered component systems that an entity has been deleted.
    void EntityManager::EntityManagerData::DestroyEntity(EntityID ID) {
        auto entityIterator = _entityComponents.find(ID);
        if (entityIterator != _entityComponents.end()) {
            _entityComponents.erase(entityIterator);
        }
        else {
            UtilityBox::Logger::LogMessage message { UtilityBox::Logger::LogMessageSeverity::WARNING };
            message.Supply("Entity at given entity ID to destroy is invalid - no entity exists at ID: %i.", ID);
            _loggingSystem.Log(message);
        }
    }

    // Destroy an entity from the Entity Manager with the provided entity name, given that it exists. Automatically notifies
    // all fully registered component systems that an entity has been deleted.
    void EntityManager::EntityManagerData::DestroyEntity(std::string name) {
        ConvertToLowercase(name);
        DestroyEntity(STRINGHASH(name.c_str()));
    }

    // Function to capture a class member function to convert it to a standard callback function callable from within
    // the Entity Manager as a response to changes to entities or their components.
    template<class Class, typename ReturnType, typename... FunctionArguments>
    inline void EntityManager::EntityManagerData::RegisterCallback(EntityManager::CallbackType callbackType, Class *classInstance, ReturnType (Class::*memberFunction)(FunctionArguments...)) {
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
    }

    // Retrieve the list of components that are attached to an entity with the provided ID, given that such an entity
    // exists in the Entity Manager.
    const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& EntityManager::EntityManagerData::GetComponents(EntityID ID) const {
        if (_entityComponents.find(ID) == _entityComponents.end()) {
            UtilityBox::Logger::LogMessage errorMessage { UtilityBox::Logger::LogMessageSeverity::SEVERE };
            errorMessage.Supply("In function GetComponents: Entity ID is invalid - no entity exists at ID: %i.", ID);
            _loggingSystem.Log(errorMessage);

            throw std::out_of_range("In function GetComponents: Entity ID is invalid - no entity exists at the provided ID.");
        }

        return _entityComponents.at(ID);
    }

    // Retrieve the list of components that are attached to an entity with the provided name, given that such an entity
    // exists in the Entity Manager.
    const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& EntityManager::EntityManagerData::GetComponents(std::string name) const {
        ConvertToLowercase(name);
        return GetComponents(STRINGHASH(name.c_str()));
    }

    // Attach a component to an entity at the provided ID, given that it exists and the entity doesn't already have a
    // component of that type attached to it. Automatically notifies all fully registered component systems that a
    // component has been added to this entity.
    template<class ComponentType>
    inline void EntityManager::EntityManagerData::AddComponent(EntityID ID) {
        // Ensure the entity with the given ID exists.
        auto entityIterator = _entityComponents.find(ID);
        if (entityIterator == _entityComponents.cend()) {
            UtilityBox::Logger::LogMessage errorMessage { UtilityBox::Logger::LogMessageSeverity::SEVERE };
            errorMessage.Supply("In function AddComponent: Entity ID provided is invalid - no entity exists at ID: %i.", ID);
            _loggingSystem.Log(errorMessage);

            throw std::invalid_argument("In function AddComponent: Entity ID provided is invalid - no entity exists at the provided ID.");
        }

        // Ensure the entity doesn't already have the component attached.
        auto entityComponentIterator = entityIterator->second.find(ComponentType::ID);
        if (entityComponentIterator != _entityComponents.end()) {
            UtilityBox::Logger::LogMessage errorMessage { UtilityBox::Logger::LogMessageSeverity::SEVERE };
            errorMessage.Supply("In function AddComponent: Entities cannot hold multiple components of the same type. Entity at ID: %i already has a component with type: '%s'.", ID, ComponentType::Name);
            _loggingSystem.Log(errorMessage);

            throw std::invalid_argument("In function AddComponent: Entities cannot hold multiple components of the same type. Entity at the provided ID already has an instance of the desired component type.");
        }
        else {

        }

        // Retrieve entity manager from the engine world, construct a component, and append it to the entity component list of the given entity.
        Components::ComponentManager<ComponentType>* componentManager = ENGINE_NAME::World::GetInstance().GetComponentManagerCollection()->GetComponentManager<ComponentType>();
        _entityComponents.at(ID).insert(ComponentType::ID, componentManager->CreateComponent());

        // Call all callback functions to notify systems.
        for (auto &componentAddCallbackFunction : _componentAddCallbackFunctions) {
            componentAddCallbackFunction(ID);
        }
    }

    // Attach a component to an entity at the provided name, given that it exists and the entity doesn't already have a
    // component of that type attached to it. Automatically notifies all fully registered component systems that a
    // component has been added to this entity.
    template<class ComponentType>
    inline void EntityManager::EntityManagerData::AddComponent(std::string name) {
        ConvertToLowercase(name);
        AddComponent<ComponentType>(STRINGHASH(name.c_str()));
    }

    // Remove a component from an entity at the provided ID, given that the entity exists in the Entity Manager. It is
    // valid behavior to delete a component from an entity that does not manage a component of that type. Automatically
    // notifies all fully registered component systems that a component has been removed from this entity.
    template<class ComponentType>
    inline void EntityManager::EntityManagerData::DeleteComponent(EntityID ID) {
        // Ensure the entity with the given ID exists.
        const auto entityIterator = _entityComponents.find(ID);
        if (entityIterator != _entityComponents.cend()) {
            // Retrieve entity manager from the engine world.
            Components::ComponentManager<ComponentType>* componentManager = ENGINE_NAME::World::GetInstance().GetComponentManagerCollection()->GetComponentManager<ComponentType>();

            // Retrieve the desired component and return it back to the component manager.
            componentManager->DeleteComponent(dynamic_cast<ComponentType*>(_entityComponents.at(ID).at(ComponentType::ID)));
            _entityComponents.at(ID).erase(ComponentType::ID);

            // Call all callback functions to notify systems.
            for (auto &componentRemoveCallbackFunction : _componentRemoveCallbackFunctions) {
                componentRemoveCallbackFunction(ID);
            }
        }
    }

    // Remove a component from an entity at the provided name, given that the entity exists in the Entity Manager. It is
    // valid behavior to delete a component from an entity that does not manage a component of that type. Automatically
    // notifies all fully registered component systems that a component has been removed from this entity.
    template<class ComponentType>
    inline void EntityManager::EntityManagerData::DeleteComponent(std::string name) {
        ConvertToLowercase(name);
        DeleteComponent<ComponentType>(STRINGHASH(name.c_str()));
    }

    template<class... ComponentTypes>
    inline bool EntityManager::EntityManagerData::CheckEntityName(const std::string& entityName) const {
        // Return true if strcasecmp returns 0 (names are equal). Strcasecmp is used to ensure lowercase letters for both
        // entity names and component names are both lowercase.
        return (!(strcasecmp(entityName.c_str(), ComponentTypes::Name)) || ...);
    }

    void EntityManager::EntityManagerData::ConvertToLowercase(std::string& string) const {
        for (char& character : string) {
            character = static_cast<char>(tolower(character));
        }
    }

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

    // Function to capture a class member function to convert it to a standard callback function callable from within
    // the Entity Manager as a response to changes to entities or their components.
    template<class Class, typename ReturnType, typename... FunctionArguments>
    inline void EntityManager::RegisterCallback(EntityManager::CallbackType callbackType, Class *classInstance, ReturnType(Class::* memberFunction)(FunctionArguments...)) {
        _data->RegisterCallback<Class, ReturnType, FunctionArguments...>(callbackType, classInstance, memberFunction);
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
        _data->AddComponent<ComponentType>(STRINGHASH(name.c_str()));
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
        _data->DeleteComponent<ComponentType>(STRINGHASH(name.c_str()));
    }
}

#endif // ENTITY_MANAGER_TPP
