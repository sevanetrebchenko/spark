
#ifndef SPARK_BASE_COMPONENT_SYSTEM_TPP
#define SPARK_BASE_COMPONENT_SYSTEM_TPP

#include "core/core.h"                         // World
#include "ecs/components/base_component.h"     // BaseComponent
#include "ecs/entities/entity_manager.h"       // EntityManager
#include "ecs/entities/entity_callback_type.h" // CallbackType

namespace Spark::ECS::Systems {
    //------------------------------------------------------------------------------------------------------------------
    // BASE COMPONENT SYSTEM DATA
    //------------------------------------------------------------------------------------------------------------------
    template <class ...ComponentTypes>
    class BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData {
        public:
            /**
             * Construct the helper function class for the Base System.
             * @param loggingSystem    - Reference to the Base System's logging system.
             * @param filteredEntities - Reference to the Base systems's filtered entities list.
             */
            BaseComponentSystemData(UtilityBox::Logger::LoggingSystem& loggingSystem, std::vector<ComponentTuple>& filteredEntities);

            /**
             * Clears helper index data used for maintaining component tuples.
             */
            ~BaseComponentSystemData();

            /**
             * Function gets called whenever an entity is created. Filters the components on the entity at creation time
             * and determines whether the entity with the given ID should be managed by this system. Gets the components
             * from the entity and emplaces a component tuple of the components managed by the system into the system store.
             * @param ID - ID of the newly added entity.
             */
            void OnEntityCreate(EntityID ID);

            /**
             * Function gets called whenever an entity is destroyed. Clears the component tuple at the provided ID and
             * ensures the remaining component tuples are still contiguous.
             * @param ID - ID of the destroyed entity.
             */
            void OnEntityDestroy(EntityID ID);

            /**
             * Function gets called whenever a component is attached to an entity. If the entity is not already managed
             * by the system, re-filters the components on the entity and determines whether the entity with the given
             * ID should be managed by this system. Gets the components from the entity and emplaces a component tuple
             * of the components managed by the system into the system store.
             * @param ID - ID of the destroyed entity.
             */
            void OnEntityComponentAdd(EntityID ID);

            /**
             * Function gets called whenever a component is removed from an entity. If the entity is managed by the
             * system, re-filters the components on the entity and determines whether the entity with the given ID should
             * still be managed by this system. If it passes the filter, component tuple remains untouched. If not,
             * clears the component tuple at the provided ID and ensures the remaining component tuples are still contiguous.
             * @param ID - ID of the entity which had a component removed.
             */
            void OnEntityComponentRemove(EntityID ID);

            /**
             * Recursive kick-off function for traversing a component tuple in search of a particular desired component
             * type.
             * @tparam DesiredComponentType    - Desired component type.
             * @tparam INDEX                   - Index of current observed tuple element.
             * @tparam ComponentType           - Current component tuple element (during component tuple traversal).
             * @tparam AdditionalComponentArgs - Remaining component tuple elements (during component tuple traversal).
             * @param  componentTuple          - Tuple to get the desired component of.
             * @param  message                 - Log message object to fill during processing.
             * @return On success: Pointer to the desired component type of the component tuple.
             *         On failure: nullptr
             */
            template <class DesiredComponentType, unsigned INDEX, class ComponentType, class ...AdditionalComponentArgs>
            DesiredComponentType* GetComponentHelper(const ComponentTuple& componentTuple, UtilityBox::Logger::LogMessage& message);

        private:
            /**
             * Filters the message at the provided ID and emplaces it into the component system tuple store if it passes.
             * @param ID      - ID of the entity to filter.
             * @param message - Log message object to fill during processing.
             */
            void VerifyEntity(EntityID ID, UtilityBox::Logger::LogMessage& message);

            /**
             * Filter entity with provided ID against the components managed by the system. If the entity with the provided
             * ID has all the required components to be processed by the system, this function also creates a component
             * tuple with its fields filled in.
             * @param ID - ID of the entity to process.
             * @return Object containing the result of filtering the entity components and the component tuple that was
             *         filled during the process.
             *         On true: Component tuple is completely constructed and in a valid state.
             *         On false: Component tuple is in an partially complete and invalid/indeterminant state.
             */
            std::pair<bool, ComponentTuple> FilterEntity(ECS::EntityID ID, UtilityBox::Logger::LogMessage& message);

            /**
             * Helper function to remove a component tuple at a given position and reconfigure maps to reflect the change.
             * @param entityToDeleteIterator - Iterator to location of the entity that needs to be removed. Function assumes
             *                                 this iterator is NOT located at one past the last element (.end()).
             * @param message                - Log message object to fill during processing.
             */
            void RemoveEntity(const std::unordered_map<EntityID, unsigned>::const_iterator& entityToDeleteIterator, UtilityBox::Logger::LogMessage& message);

            /**
             * Recursive kick-off function for determining whether a component attached to an entity is part of the system's
             * component tuple or not. If the component the system cares about exists in the entity's component list, it
             * gets filled in the tuple.
             * @tparam INDEX                   - Index of current observed tuple element.
             * @tparam ComponentType           - Current component tuple element (during component tuple traversal).
             * @tparam AdditionalComponentArgs - Remaining component tuple elements (during component tuple traversal).
             * @param  componentTypeID         - ID of the current entity component being processed.
             * @param  component               - Pointer to the base component at the componentTypeID (used for casting
             *                                   and filling the component tuple data).
             * @param  componentTuple          - Component tuple being constructed.
             * @param  message                 - Log message object to fill during processing.
             * @return On success: Function fills in the component in the component tuple.
             *         On failure: Function does nothing.
             */
            template <unsigned INDEX, class ComponentType, class ...AdditionalComponentArgs>
            bool ProcessEntityComponent(ComponentTypeID componentTypeID, Components::BaseComponent* component, ComponentTuple& componentTuple, UtilityBox::Logger::LogMessage& message);

            /**
             * Recursive base-case function for processing an component from the entity's component list. If this function
             * is reached, the chosen component is not managed by this system.
             * @param message - Log message object to fill during processing.
             * @return Always false.
             */
            template <unsigned>
            bool ProcessEntityComponent(ComponentTypeID, Components::BaseComponent*, ComponentTuple&, UtilityBox::Logger::LogMessage& message);

            UtilityBox::Logger::LoggingSystem& _loggingSystemReference; // Reference to Base System logging system.
            std::vector<ComponentTuple>& _filteredEntitiesReference;    // Reference to Base System filtered entities.

            std::unordered_map<EntityID, unsigned> _entityIDToContainerIndex; // Mapping from entity ID to index in filtered entities vector.
            std::unordered_map<unsigned, EntityID> _containerIndexToEntityID; // Mapping from index in filtered entities vector to entity ID.
    };

    // Construct the helper function class for the Base System.
    template<class... ComponentTypes>
    inline BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::BaseComponentSystemData(UtilityBox::Logger::LoggingSystem& loggingSystem, std::vector<ComponentTuple>& filteredEntities) : _loggingSystemReference(loggingSystem),
                                                                                                                                                                                                       _filteredEntitiesReference(filteredEntities)
                                                                                                                                                                                                       {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Constructing BaseComponentSystem.");
        // Register callback functions.
        Entities::EntityManager *entityManager = Spark::Core::GetInstance()->GetEntityManager();

        // Register callback for when entities get created.
        message.Supply("Registering 'OnEntityCreate' callback function.");
        entityManager->RegisterCallback<BaseComponentSystemData, void, EntityID>(Entities::CallbackType::ENTITY_CREATE, this, &BaseComponentSystemData::OnEntityCreate);

        // Register callback for when entities get deleted.
        message.Supply("Registering 'OnEntityDestroy' callback function.");
        entityManager->RegisterCallback<BaseComponentSystemData, void, EntityID>(Entities::CallbackType::ENTITY_DELETE, this, &BaseComponentSystemData::OnEntityDestroy);

        // Register callback for when a component is attached to an entity.
        message.Supply("Registering 'OnEntityComponentAdd' callback function.");
        entityManager->RegisterCallback<BaseComponentSystemData, void, EntityID>(Entities::CallbackType::COMPONENT_ADD, this, &BaseComponentSystemData::OnEntityComponentAdd);

        // Register callback for when a component is removed from an entity.
        message.Supply("Registering 'OnEntityComponentRemove' callback function.");
        entityManager->RegisterCallback<BaseComponentSystemData, void, EntityID>(Entities::CallbackType::COMPONENT_REMOVE, this, &BaseComponentSystemData::OnEntityComponentRemove);

        _loggingSystemReference.Log(message);
    }

    // Clears helper index data used for maintaining component tuples.
    template<class... ComponentTypes>
    inline BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::~BaseComponentSystemData() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Destroying BaseComponentSystem.");
        _loggingSystemReference.Log(message);

        _containerIndexToEntityID.clear();
        _entityIDToContainerIndex.clear();
    }

    // Function gets called whenever an entity is created. Filters the components on the entity at creation time and
    // determines whether the entity with the given ID should be managed by this system. Gets the components from the
    // entity and emplaces a component tuple of the components managed by the system into the system store.
    template<class... ComponentTypes>
    inline void BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::OnEntityCreate(EntityID ID) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entered function OnEntityCreate with entity ID: %i.", ID);

        // Verify if the desired entity's components pass the filter. Construct and add the component tuple on success.
        message.Supply("Verifying entity to make sure entity has the necessary components to be managed by this component system.");
        VerifyEntity(ID, message);

        _loggingSystemReference.Log(message);
    }

    // Function gets called whenever an entity is destroyed. Clears the component tuple at the provided ID and ensures
    // the remaining component tuples are still contiguous.
    template<class... ComponentTypes>
    inline void BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::OnEntityDestroy(EntityID ID) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function OnEntityDestroy with entity ID: %i.", ID);

        message.Supply("Attempting to find entity with provided ID in this system's list of managed component tuples.");
        auto entityToDeleteIterator = _entityIDToContainerIndex.find(ID); // EntityID, index

        // Remove the component tuple associated with the entity if it exists in and is managed by this system.
        if (entityToDeleteIterator != _entityIDToContainerIndex.end()) {
            message.Supply("Removing entity with the provided ID from this system's list of managed component tuples.");
            RemoveEntity(entityToDeleteIterator, message);
        }
        else {
            message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::WARNING);
            message.Supply("Entity location was not found in the system component list - no entity exists at ID: %i.", ID);
            _loggingSystemReference.Log(message);
        }

        _loggingSystemReference.Log(message);
    }

    // Function gets called whenever a component is attached to an entity. If the entity is not already managed by the
    // system, re-filters the components on the entity and determines whether the entity with the given ID should be
    // managed by this system. Gets the components from the entity and emplaces a component tuple of the components
    // managed by the system into the system store.
    template<class... ComponentTypes>
    inline void BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::OnEntityComponentAdd(EntityID ID) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function OnEntityComponentAdd with entity ID: %i.", ID);

        // Perform entity filtering if the entity with the given ID is not already part of the system.
        if (_entityIDToContainerIndex.find(ID) == _entityIDToContainerIndex.end()) {
            message.Supply("Entity with provided ID was found within the system. Proceeding to component filtering.");
            VerifyEntity(ID, message);
        }
        else {
            message.Supply("Entity already has the required components and is being tracked by this system. No further action necessary.");
        }

        _loggingSystemReference.Log(message);
    }

    // Function gets called whenever a component is removed from an entity. If the entity is managed by the system,
    // re-filters the components on the entity and determines whether the entity with the given ID should still be
    // managed by this system. If it passes the filter, component tuple remains untouched. If not, clears the component
    // tuple at the provided ID and ensures the remaining component tuples are still contiguous.
    template<class... ComponentTypes>
    inline void BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::OnEntityComponentRemove(EntityID ID) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function OnEntityComponentRemove with entity ID: %i.", ID);

        const auto& entityToDeleteIterator = _entityIDToContainerIndex.find(ID); // EntityID, index

        // Ensure component tuple with provided ID exists in the managed component tuples of the system.
        if (entityToDeleteIterator != _entityIDToContainerIndex.end()) {
            message.Supply("Entity component tuple found within this system. Proceeding to entity filtering to check if entity has the components to be managed by this system.");
            std::pair<bool, ComponentTuple> filterResult = FilterEntity(ID, message);

            // If the entity passes filtering, make no changes to the system component tuple list.
            if (filterResult.first) {
                message.Supply("Removed component was not a component managed by this system. No changes need to be made to the component tuple.");
            }
            // If the entity fails filtering, remove it from the system.
            else {
                message.Supply("Removed component was a component managed by this system. Entity is no longer valid to be managed by this system - removing entity with the provided ID from this system's list of managed component tuples.");
                RemoveEntity(entityToDeleteIterator, message);
            }
        }
        else {
            message.Supply("Entity is not being tracked by the system. No further action necessary.");
        }

        _loggingSystemReference.Log(message);
    }

    // Recursive kick-off function for traversing a component tuple in search of a particular desired component type.
    template<class... ComponentTypes>
    template<class DesiredComponentType, unsigned INDEX, class ComponentType, class ...AdditionalComponentArgs>
    inline DesiredComponentType* BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::GetComponentHelper(const BaseComponentSystem::ComponentTuple& componentTuple, UtilityBox::Logger::LogMessage &message) {
        // Recursion should always find component as the desired component is guaranteed to be managed by this system and
        // component tuples that don't have the component attached (or a nullptr) are automatically not managed by this system.
        if constexpr (DesiredComponentType::ID == ComponentType::ID) {
            message.Supply("Found desired component type: '%s' at tuple index: %i.", ComponentType::Name, INDEX);
            return std::get<INDEX>(componentTuple);
        }
        else {
            message.Supply("Component type: '%s' at tuple index: %i does not match desired component type: '%s'.", ComponentType::Name, INDEX, DesiredComponentType::Name);
            return GetComponentHelper<DesiredComponentType, INDEX + 1, AdditionalComponentArgs...>(componentTuple, message);
        }
    }

    // Filters the message at the provided ID and emplaces it into the component system tuple store if it passes.
    template<class... ComponentTypes>
    inline void BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::VerifyEntity(EntityID ID, UtilityBox::Logger::LogMessage& message) {
        message.Supply("Entering function VerifyEntity with entity ID: %i.", ID);

        // Attempt to filter entity by components.
        message.Supply("Proceeding to entity filtering to check if entity has the components to be managed by this system.");
        std::pair<bool, ComponentTuple> filterResult = FilterEntity(ID, message);

        // Entity passed component filtering.
        if (filterResult.first) {
            unsigned filteredEntityIndex = _filteredEntitiesReference.size() - 1;
            message.Supply("Entity passed component filtering. Component tuple of required components will be added to base component system's filtered component list at position: %i.", filteredEntityIndex);
            _filteredEntitiesReference.emplace_back(std::move(filterResult.second));

            // Set up mapping for further ease-of-access.
            message.Supply("Setting up indices in Base Component System maps for future access.");
            _entityIDToContainerIndex.emplace(ID, filteredEntityIndex);
            _containerIndexToEntityID.emplace(filteredEntityIndex, ID);
        }
        // Entity failed component filtering.
        else {
            message.Supply("Entity failed to pass component filtering function.");
        }
    }

    // Filter entity with provided ID against the components managed by the system. If the entity with the provided ID
    // has all the required components to be processed by the system, this function also creates a component tuple with
    // its fields filled in.
    template<class... ComponentTypes>
    inline std::pair<bool, typename BaseComponentSystem<ComponentTypes...>::ComponentTuple> BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::FilterEntity(EntityID ID, UtilityBox::Logger::LogMessage& message) {
        message.Supply("Entering function FilterEntity with entity ID: %s.", ID);
        ComponentTuple componentTuple;
        unsigned numMatchingComponents = 0;
        Entities::EntityManager* entityManager = Spark::Core::GetInstance()->GetEntityManager();

        for (const auto& entityComponentPair : entityManager->GetComponents(ID)) { // ComponentTypeID, BaseComponent*
            // Attempt to see if this component is present in the system's component list.
            if (ProcessEntityComponent<0, ComponentTypes...>(entityComponentPair.first, entityComponentPair.second, componentTuple, message)) {
                // Component was matched and filled in the component tuple.
                ++numMatchingComponents;

                // If the number of matching components is equal to the total number of components the system manages,
                // the component tuple is fully constructed and can be appended to the list of components.
                if (numMatchingComponents == sizeof...(ComponentTypes)) {
                    message.Supply("Component requirement for this system fulfilled, component tuple fully constructed.");
                    return std::make_pair(true, std::move(componentTuple));
                }
            }
        }
        // Entity is not managed by this system.
        message.Supply("Component requirement for this system is not fulfilled, component tuple is in an invalid/indeterminate state.");
        return std::make_pair(false, std::move(componentTuple));
    }

    // Helper function to remove a component tuple at a given position and reconfigure maps to reflect the change.
    template<class... ComponentTypes>
    inline void BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::RemoveEntity(const std::unordered_map<EntityID, unsigned>::const_iterator& entityToDeleteIterator, UtilityBox::Logger::LogMessage& message) {
        message.Supply("Entering function VerifyEntity with entity ID: %i.", entityToDeleteIterator->first);

        unsigned toDeleteIndex = entityToDeleteIterator->second; // Index of the component tuple to delete.
        unsigned lastEntityIndex = _filteredEntitiesReference.size() - 1; // Index of the last component tuple.

        // Get the ID of the component tuple at the last index for later use.
        auto lastEntityIterator = _containerIndexToEntityID.find(lastEntityIndex); // index, EntityID
        EntityID lastEntityID = lastEntityIterator->second;

        // Delete component tuple.
        // Overwrite the component tuple marked for deletion with the last one and decrease container size by 1.
        message.Supply("Overwriting component tuple at index: %i with component tuple at index: %i.", toDeleteIndex, lastEntityIndex);
        _filteredEntitiesReference[toDeleteIndex] = std::move(_filteredEntitiesReference[lastEntityIndex]);
        _filteredEntitiesReference.pop_back();

        // Clear index and ID from respective maps.
        _containerIndexToEntityID.erase(lastEntityIndex); // Array is shrinking from the back, last index is invalid.
        _entityIDToContainerIndex.erase(entityToDeleteIterator->first); // Tuple with entity ID 'ID' no longer exists.

        // Update index and ID maps to reflect change.
        auto IDMapIterator = _entityIDToContainerIndex.find(lastEntityID); // EntityID, index
        IDMapIterator->second = toDeleteIndex;
        auto indexMapIterator = _containerIndexToEntityID.find(toDeleteIndex); // index, EntityID
        indexMapIterator->second = lastEntityID;

        message.Supply("Component tuple that used to be at index: %i is now at index: %i. Maps have been updated to reflect change.", lastEntityIndex, toDeleteIndex);
    }

    // Recursive kick-off function for determining whether a component attached to an entity is part of the system's
    // component tuple or not. If the component the system cares about exists in the entity's component list, it gets
    // filled in the tuple.
    template<class... ComponentTypes>
    template<unsigned int INDEX, class ComponentType, class... AdditionalComponentArgs>
    inline bool BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::ProcessEntityComponent(ComponentTypeID componentTypeID, Components::BaseComponent *component, BaseComponentSystem::ComponentTuple &componentTuple, UtilityBox::Logger::LogMessage& message) {
        message.Supply("Attempting to pair component: '%s' from entity component list into component tuple.", ComponentType::Name);

        // ID's match, we found a component in the entity that is included in the ComponentTuple that the system manages.
        if (ComponentType::ID == componentTypeID) {
            // We know the component exists, dynamic cast should always succeed.
            auto* derivedComponentType = dynamic_cast<ComponentType*>(component);
            ASSERT(derivedComponentType != nullptr, "System failed to get matching component."); // Should never happen.

            std::get<INDEX>(componentTuple) = derivedComponentType;
            message.Supply("Pairing success - entity component '%s' added to component tuple.", ComponentType::Name);

            return true;
        }
        else {
            message.Supply("Pairing failed - component types do not match. Moving on to next tuple position.");
            return ProcessEntityComponent<INDEX + 1, AdditionalComponentArgs...>(componentTypeID, component, componentTuple, message);
        }
    }

    // Recursive base-case function for processing an component from the entity's component list. If this function is
    // reached, the chosen component is not managed by this system.
    template<class... ComponentTypes>
    template<unsigned> // Unused.
    inline bool BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::ProcessEntityComponent(ComponentTypeID /* Unused. */, Components::BaseComponent* /* Unused. */, BaseComponentSystem::ComponentTuple& /* Unused. */, UtilityBox::Logger::LogMessage& message) {
        // There are no more components that the system manages to go through, 'component' is not managed by this system.
        message.Supply("Component type is not managed by this system. No changes made to component tuple.");
        return false;
    }

    //------------------------------------------------------------------------------------------------------------------
    // BASE COMPONENT SYSTEM
    //------------------------------------------------------------------------------------------------------------------
    // Constructs the necessary fundamentals for a component system, including automatic entity filtering and logging
    // system instance. Enforces all component types managed by this system are derived from BaseComponent.
    template <class... ComponentTypes>
    inline BaseComponentSystem<ComponentTypes...>::BaseComponentSystem(std::string&& systemName) : _systemName(std::move(systemName)),
                                                                                                   _data(new BaseComponentSystemData(_loggingSystem, _filteredEntities)) /* Throws exception, not caught purposefully. */
                                                                                                   {
        // Ensure all components are derived from base component.
        static_assert((std::is_base_of_v<Components::BaseComponent, ComponentTypes> && ...), "Invalid template parameter provided to base BaseComponentSystem - component types must derive from ecs::components::BaseComponent.");
    }

    // Destructor.
    template <class... ComponentTypes>
    inline BaseComponentSystem<ComponentTypes...>::~BaseComponentSystem() {
        delete _data;
    }

    // Initialize the logging system and any supplemental back-end functionality.
    template<class... ComponentTypes>
    inline void BaseComponentSystem<ComponentTypes...>::Initialize() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function Initialize (for BaseComponentSystem).");
        _loggingSystem.Log(message);
    }

    // Update.
    template<class... ComponentTypes>
    inline void BaseComponentSystem<ComponentTypes...>::Update(float dt) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function Update (for BaseComponentSystem).");
        _loggingSystem.Log(message);
    }

    // Shutdown any resources associated with this component system.
    template <class... ComponentTypes>
    inline void BaseComponentSystem<ComponentTypes...>::Shutdown() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function Shutdown (for BaseComponentSystem).");

        // Clear system data.
        if (!_filteredEntities.empty()) {
            _filteredEntities.clear();
        }
        delete _data;

        _loggingSystem.Log(message);
    }

    // Get a specific component pointer from the tuple at the provided index. Function ensures the desired component is
    // managed by the system.
    template <class... ComponentTypes>
    template <class ComponentType>
    inline ComponentType* BaseComponentSystem<ComponentTypes...>::GetComponent(unsigned index) {
        // Ensure component is managed by this system.
        static_assert((std::is_same_v<ComponentType, ComponentTypes> || ...), "Invalid template parameter provided to base GetComponentHelper - component type is not managed by the queried system.");

        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function GetComponent with index: %i.", index);

        // Check for invalid index.
        if (index >= _filteredEntities.size()) {
            message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::SEVERE);
            message.Supply("Exception thrown: Index: %i provided to GetComponent is out of range.", index);
            _loggingSystem.Log(message);

            throw std::out_of_range("Invalid index provided to GetComponent.");
        }

        message.Supply("Index is valid. Retrieving component of type: '%s' from index %i.", ComponentType::Name, index);
        ComponentType* component = _data->template GetComponentHelper<ComponentType, 0, ComponentTypes...>(_filteredEntities[index], message);
        _loggingSystem.Log(message);
        return component;
    }

    // Get a specific component pointer from the tuple at the provided index. Function ensures the desired component is
    // managed by the system.
    template <class... ComponentTypes>
    template <class ComponentType>
    inline ComponentType* BaseComponentSystem<ComponentTypes...>::GetComponent(const BaseComponentSystem::ComponentTuple& componentTuple) {
        // Ensure component is managed by this system.
        static_assert((std::is_same_v<ComponentType, ComponentTypes> || ...), "Invalid template parameter provided to base GetComponentHelper - component type is not managed by the queried system.");

        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function GetComponent with existing tuple. Retrieving component of type: '%s'.", ComponentType::Name);

        ComponentType* component = _data->template GetComponentHelper<ComponentType, 0, ComponentTypes...>(componentTuple, message);
        _loggingSystem.Log(message);
        return component;
    }
}

#endif // SPARK_BASE_COMPONENT_SYSTEM_TPP
