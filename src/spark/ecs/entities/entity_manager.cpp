
#include <spark/ecs/entities/entity_manager.h>                 // EntityManager
#include <spark/ecs/components/component_manager.h>            // ComponentManager
#include <spark/ecs/components/component_manager_collection.h> // ComponentManagerCollection

namespace Spark::ECS::Entities {
    //------------------------------------------------------------------------------------------------------------------
    // ENTITY MANAGER DATA
    //------------------------------------------------------------------------------------------------------------------
    class EntityManager::EntityManagerData : public UtilityBox::Logger::ILoggable {
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
            * Create an entity. Throws error if the provided entity name matches any of the the bin-in component type
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
             * Register a callback function of a given type with the entity manager.
             * @param callbackType     - Callback function type.
             * @param callbackFunction - Callback function.
             */
            void RegisterCallback(CallbackType callbackType, const std::function<void(EntityID)>& callbackFunction);

            /**
            * Retrieve a map of all entities and their attached components.
            * @return A map of all entities and their attached components.
            */
            _NODISCARD_ const std::unordered_map<EntityID, std::unordered_map<ComponentTypeID, Components::BaseComponent *>>& GetComponentMap() const;

            /**
            * Retrieve the list of components that are attached to an entity with the provided ID, given that such an
            * entity exists in the Entity Manager.
            * @param ID - ID of the entity to get the components of.
            * @return List of attached components to the entity at the provided ID.
            */
            _NODISCARD_ const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& GetComponents(EntityID ID) const;

            /**
            * Retrieve the list of components that are attached to an entity with the provided name, given that such an
            * entity exists in the Entity Manager.
            * @param name - Name of the entity to get the components of.
            * @return List of attached components to the entity at the provided name.
            */
            _NODISCARD_ const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& GetComponents(std::string name) const;

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
            * Remove a component from an entity at the provided ID, given that the entity exists in the Entity Manager.
            * It is valid behavior to delete a component from an entity that does not manage a component of that type.
            * Automatically notifies all fully registered component systems that a component has been removed from this
            * entity.
            * @tparam ComponentType - Type of component to attach.
            * @param  ID            - ID of the entity from which to remove the component.
            */
            template <class ComponentType>
            void RemoveComponent(EntityID ID);

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
            * Convert an instance of an EntityManager::CallbackType object to a string to use in debug messages.
            * @param callbackType - Type of callback function to convert.
            * @return String representation of the callback function type.
            */
            const char* CallbackTypeToString(CallbackType callbackType);

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
    };

    EntityManager* EntityManager::_instance = nullptr;

    // Constructor.
    EntityManager::EntityManagerData::EntityManagerData() : UtilityBox::Logger::ILoggable("Entity Manager") {
        LogDebug("Constructing EntityManager.");
    }

    // Cleans up any resources associated with the Entity Manager helper class.
    EntityManager::EntityManagerData::~EntityManagerData() {
        LogDebug("Destroying EntityManager.");

        // Clear entity components.
        for (auto entityComponentList : _entityComponents) {
            entityComponentList.second.clear();
        }
        _entityComponents.clear();

        // Clear entity names.
        _entityNames.clear();

        // Clear callback functions.
        _entityCreateCallbackFunctions.clear();
        _entityDestroyCallbackFunctions.clear();
        _componentAddCallbackFunctions.clear();
        _componentRemoveCallbackFunctions.clear();
    }

    // Create an entity. Throws error if the provided entity name matches any of the the bin-in component type names
    // or any pre-existing entity names. Automatically notifies all fully registered component systems that a new entity has been created.
    void EntityManager::EntityManagerData::CreateEntity(std::string name) {
        LogDebug("Entering function CreateEntity with entity name: '%s.'", name.c_str());

        ConvertToLowercase(name);
        LogDebug("Entity name converted to lowercase: '%s.'", name.c_str());

        // Entity name cannot be a name of any component.
        LogDebug("Checking name against the names of all built-in component names.");

        if (CheckEntityName<ALL_COMPONENTS>(name)) {
            LogError("Exception thrown: Provided entity name: '%s' cannot match a built-in component name.", name.c_str());
            throw std::invalid_argument("In function CreateEntity: Provided entity name cannot match a built-in component name.");
        }

        EntityID hashedID = STRINGHASH(name.c_str());
        LogDebug("Checking hashed entity ID against all other entities.");

        // Newly created entity cannot have a matching ID to an already existing entity.
        if (_entityComponents.find(hashedID) != _entityComponents.end()) {
            LogError("Exception thrown: Hashed entity ID matches with another entity: there already exists an entity with the same hashed ID under entity name: '%s'.", _entityNames.find(hashedID)->second.c_str());
            throw std::invalid_argument("In function CreateEntity: Provided entity name matches an already existing entity name.");
        }

        LogDebug("Entity creation successful. Creating entries for entity component list and name.");
        // Index operator creates a new default constructed entry.
        _entityComponents[hashedID];
        _entityNames[hashedID] = name;

        // Notify systems of created entity.
        LogDebug("Component system 'OnEntityCreate' callback functions called.");

        for (auto &entityCreateCallbackFunction : _entityCreateCallbackFunctions) {
            entityCreateCallbackFunction(hashedID);
        }
    }

    // Destroy an entity from the Entity Manager with the provided entity ID, given that it exists. Automatically notifies
    // all fully registered component systems that an entity has been deleted.
    void EntityManager::EntityManagerData::DestroyEntity(EntityID ID) {
        LogDebug("Entering function DestroyEntity with entity ID: %i.", ID);

        auto entityIterator = _entityComponents.find(ID);
        if (entityIterator != _entityComponents.end()) {
            _entityComponents.erase(entityIterator);
            LogDebug("Entity location was found and removed from Entity Manager.");

            // Notify systems of deleted entity.
            LogDebug("Component system 'OnEntityDestroy' callback functions called.");

            for (auto &entityDestroyCallbackFunction : _entityDestroyCallbackFunctions) {
                entityDestroyCallbackFunction(ID);
            }
        }
        else {
            LogWarning("Entity location was not found in entity manager - no entity exists at ID: %i.", ID);
        }
    }

    // Destroy an entity from the Entity Manager with the provided entity name, given that it exists. Automatically notifies
    // all fully registered component systems that an entity has been deleted.
    void EntityManager::EntityManagerData::DestroyEntity(std::string name) {
        LogDebug("Entering function DestroyEntity with entity name: '%s.'", name.c_str());

        ConvertToLowercase(name);
        LogDebug("Entity name converted to lowercase: '%s.'", name.c_str());

        // Hand off responsibility to overloaded function.
        LogDebug("Calling function DestroyEntity with hashed entity ID.");

        DestroyEntity(STRINGHASH(name.c_str()));
    }

    // Function to capture a class member function to convert it to a standard callback function callable from within
    // the Entity Manager as a response to changes to entities or their components.
    void EntityManager::EntityManagerData::RegisterCallback(CallbackType callbackType, const std::function<void(EntityID)>& callbackFunction) {
        const char* callbackTypeToString = CallbackTypeToString(callbackType);
        LogDebug("Entering function RegisterCallback with component system callback type: '%s.'", callbackTypeToString);

        // Append the function to the correct location.
        switch (callbackType) {
            case CallbackType::ENTITY_CREATE:
                _entityCreateCallbackFunctions.emplace_back(callbackFunction);
                break;
            case CallbackType::ENTITY_DELETE:
                _entityDestroyCallbackFunctions.emplace_back(callbackFunction);
                break;
            case CallbackType::COMPONENT_ADD:
                _componentAddCallbackFunctions.emplace_back(callbackFunction);
                break;
            case CallbackType::COMPONENT_REMOVE:
                _componentRemoveCallbackFunctions.emplace_back(callbackFunction);
                break;
        }

        LogDebug("Successfully enabled tracking for member callback function of type: '%s'.", callbackTypeToString);
    }

    // Retrieve the list of components that are attached to an entity with the provided ID, given that such an entity
    // exists in the Entity Manager.
    const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& EntityManager::EntityManagerData::GetComponents(EntityID ID) const {
        LogDebug("Entering function GetComponents with entity ID: %i.", ID);

        if (_entityComponents.find(ID) == _entityComponents.end()) {
            LogError("Exception thrown: Entity location was not found in entity manager - no entity exists at ID: %i.", ID);
            throw std::out_of_range("In function GetComponents: Entity ID is invalid - no entity exists at the provided ID.");
        }

        LogDebug("Entity found, components accessed.");

        return _entityComponents.at(ID);
    }

    // Retrieve the list of components that are attached to an entity with the provided name, given that such an entity
    // exists in the Entity Manager.
    const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& EntityManager::EntityManagerData::GetComponents(std::string name) const {
        LogDebug("Entering function GetComponents with entity name: '%s.'", name.c_str());

        ConvertToLowercase(name);
        LogDebug("Entity name converted to lowercase: '%s.'", name.c_str());

        // Hand off responsibility to overloaded function.
        LogDebug("Calling function GetComponents with hashed entity ID.");
        return GetComponents(STRINGHASH(name.c_str()));
    }

    // Attach a component to an entity at the provided ID, given that it exists and the entity doesn't already have a
    // component of that type attached to it. Automatically notifies all fully registered component systems that a
    // component has been added to this entity.
    template<class ComponentType>
    inline void EntityManager::EntityManagerData::AddComponent(EntityID ID) {
        static_assert(std::is_base_of<Components::BaseComponent, ComponentType>::value, "In function AddComponent: Invalid component type provided.");

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

    // Remove a component from an entity at the provided ID, given that the entity exists in the Entity Manager. It is
    // valid behavior to delete a component from an entity that does not manage a component of that type. Automatically
    // notifies all fully registered component systems that a component has been removed from this entity.
    template<class ComponentType>
    inline void EntityManager::EntityManagerData::RemoveComponent(EntityID ID) {
        static_assert(std::is_base_of<Components::BaseComponent, ComponentType>::value, "In function RemoveComponent: Invalid component type provided.");

        LogDebug("Entering function RemoveComponent with entity ID: %i.", ID);

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

    // Check the name of the given entity name against the names of all of the built-in components to ensure the names
    // do not overlap. Names are all converted to lowercase before comparison.
    template<class... ComponentTypes>
    inline bool EntityManager::EntityManagerData::CheckEntityName(const std::string& entityName) const {
        // Return true if strcasecmp returns 0 (names are equal). Strcasecmp is used to ensure lowercase letters for both
        // entity names and component names are both lowercase.
        return (!(strcasecmp(entityName.c_str(), ComponentTypes::Name)) || ...);
    }

    // Convert a given string to lowercase.
    void EntityManager::EntityManagerData::ConvertToLowercase(std::string& string) const {
        // Character by character traversal + conversion.
        for (char& character : string) {
            character = static_cast<char>(tolower(character));
        }
    }

    // Convert an instance of an EntityManager::CallbackType object to a string to use in debug messages.
    const char* EntityManager::EntityManagerData::CallbackTypeToString(CallbackType callbackType) {
        switch (callbackType) {
            case CallbackType::ENTITY_CREATE:
                return "OnEntityCreate";
            case CallbackType::ENTITY_DELETE:
                return "OnEntityDestroy";
            case CallbackType::COMPONENT_ADD:
                return "OnEntityComponentAdd";
            case CallbackType::COMPONENT_REMOVE:
                return "OnEntityComponentRemove";
            default:
                return "Unknown type";
        }
    }

    const std::unordered_map<EntityID, std::unordered_map<ComponentTypeID, Components::BaseComponent *>>& EntityManager::EntityManagerData::GetComponentMap() const {
        return _entityComponents;
    }


    //------------------------------------------------------------------------------------------------------------------
    // ENTITY MANAGER
    //------------------------------------------------------------------------------------------------------------------
    EntityManager *EntityManager::GetInstance() {
        if (!_instance) {
            _instance = new EntityManager();
        }

        return _instance;
    }

    EntityManager::EntityManager() : IEventReceivable("Entity Manager"), _data(new EntityManagerData()) /* Throws on error, caught elsewhere. */ {
        // Nothing to do here.
    }

    // Cleans up any resources associated with the Entity Manager.
    EntityManager::~EntityManager() {
        delete _data;
    }

    void EntityManager::RegisterCallback(CallbackType callbackType, const std::function<void(EntityID)>& callbackFunction) {
        _data->RegisterCallback(callbackType, callbackFunction);
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

    // Convert a given entity name to the relative entity ID.
    EntityID EntityManager::GetEntityIDFromName(const std::string &entityName) const {
        return STRINGHASH(entityName.c_str());
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

    void EntityManager::OnEvent(Events::AddComponentEvent<Components::TestComponent> *event) {
        _data->AddComponent<Components::TestComponent>(event->GetEntityID());
    }

    void EntityManager::OnEvent(Events::RemoveComponentEvent<Components::TestComponent> *event) {
        _data->RemoveComponent<Components::TestComponent>(event->GetEntityID());
    }

    const std::unordered_map<EntityID, std::unordered_map<ComponentTypeID, Components::BaseComponent *>>& EntityManager::GetComponentMap() const {
        return _data->GetComponentMap();
    }

}
