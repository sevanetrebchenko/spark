
#ifndef BASE_COMPONENT_SYSTEM_TPP
#define BASE_COMPONENT_SYSTEM_TPP

#include "../Components/base_component.h"
#include "../Entities/entity_manager.h"
#include "../../World/world.h"

namespace ECS::Systems {
    template <class ...ComponentTypes>
    class BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData {
        public:
            /**
             * Construct the helper function class for the Base System.
             * @param loggingSystem    - Reference to the Base System's logging system.
             * @param filteredEntities - Reference to the Base Systems's filtered entities list.
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
             * @return On success: Pointer to the desired component type of the component tuple.
             *         On failure: nullptr
             */
            template <class DesiredComponentType, unsigned INDEX, class ComponentType, class ...AdditionalComponentArgs>
            DesiredComponentType* GetComponentHelper(const ComponentTuple& componentTuple);

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

    template <class... ComponentTypes>
    inline BaseComponentSystem<ComponentTypes...>::BaseComponentSystem(std::string&& systemName) : _systemName(std::move(systemName)) {
        static_assert((std::is_base_of<Components::BaseComponent, ComponentTypes>::value && ...), "Invalid template parameter provided to base BaseComponentSystem - component types must derive from ECS::Components::BaseComponent.");

        // Defer initialization to second stage.
    }

    template<class... ComponentTypes>
    inline void BaseComponentSystem<ComponentTypes...>::Initialize() {
        // Constructor for BaseComponentSystemData automatically registers component callback functions.
        _data = new BaseComponentSystemData(_loggingSystem, _filteredEntities);

        UtilityBox::Logger::LogMessage message {};
        message.Supply("Initializing Base System.");
        _loggingSystem.Log(message);
    }

    template<class... ComponentTypes>
    inline void BaseComponentSystem<ComponentTypes...>::Update(float dt) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Update called for Base System.");
        _loggingSystem.Log(message);
    }

    template <class... ComponentTypes>
    inline void BaseComponentSystem<ComponentTypes...>::Shutdown() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Shutdown called for Base System.");
        _loggingSystem.Log(message);

        // Clear system data.
        if (!_filteredEntities.empty()) {
            _filteredEntities.clear();
        }

        delete _data;
    }

    template<class... ComponentTypes>
    inline void BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::OnEntityCreate(EntityID ID) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entered function OnEntityCreate with entity ID: %i.", ID);

        // Verify if the desired entity's components pass the filter. Construct and add the component tuple on success.
        VerifyEntity(ID, message);

        _loggingSystemReference.Log(message);
    }

    template<class... ComponentTypes>
    inline void BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::OnEntityDestroy(EntityID ID) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entered function OnEntityDestroy with entity ID: %i.", ID);

        auto entityToDeleteIterator = _entityIDToContainerIndex.find(ID); // EntityID, index

        // Remove the component tuple associated with the entity if it exists in and is managed by this system.
        if (entityToDeleteIterator != _entityIDToContainerIndex.end()) {
            RemoveEntity(entityToDeleteIterator, message);
        }

        _loggingSystemReference.Log(message);
    }

    template<class... ComponentTypes>
    inline void BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::OnEntityComponentAdd(EntityID ID) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entered function OnEntityComponentAdd with entity ID: %i.", ID);

        // Perform entity filtering if the entity with the given ID is not already part of the system.
        if (_entityIDToContainerIndex.find(ID) == _entityIDToContainerIndex.end()) {
            VerifyEntity(ID, message);
        }
        else {
            message.Supply("Entity already has the required components and is being tracked by this system.");
        }

        _loggingSystemReference.Log(message);
    }

    template<class... ComponentTypes>
    inline void BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::OnEntityComponentRemove(EntityID ID) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entered function OnEntityComponentRemove with entity ID: %i.", ID);

        // Find the location of the component tuple in the filtered entities array that belongs to the entity.
        const auto& entityToDeleteIterator = _entityIDToContainerIndex.find(ID); // EntityID, index

        // Ensure component tuple with provided ID exists in the managed component tuples of the system.
        if (entityToDeleteIterator != _entityIDToContainerIndex.end()) {
            message.Supply("Entity component tuple found within this system.");

            // Filter entity.
            std::pair<bool, ComponentTuple> filterResult = FilterEntity(ID, message);

            // If the entity passes filtering, make no changes to the system component tuple list.
            if (filterResult.first) {
                message.Supply("Removed component was not a component managed by this system. No changes need to be made to the component tuple.");
                _loggingSystemReference.Log(message);
                return;
            }

            RemoveEntity(entityToDeleteIterator, message);
            _loggingSystemReference.Log(message);
        }
    }

    template <class... ComponentTypes>
    template <class ComponentType>
    inline ComponentType* BaseComponentSystem<ComponentTypes...>::GetComponent(unsigned index) {
        static_assert((std::is_same_v<ComponentType, ComponentTypes> || ...), "Invalid template parameter provided to base GetComponentHelper - component type is not managed by the queried system.");

        if (index >= _filteredEntities.size()) {
            UtilityBox::Logger::LogMessage errorMessage { UtilityBox::Logger::LogMessageSeverity::SEVERE };
            errorMessage.Supply("Exception thrown: Index: %i provided to GetComponent is invalid.", index);
            _loggingSystem.Log(errorMessage);

            throw std::out_of_range("Invalid index provided to GetComponent.");
        }

        return _data->template GetComponentHelper<ComponentType, 0, ComponentTypes...>(_filteredEntities[index]);
    }

    template <class... ComponentTypes>
    template <class ComponentType>
    inline ComponentType* BaseComponentSystem<ComponentTypes...>::GetComponent(const BaseComponentSystem::ComponentTuple& componentTuple) {
        static_assert((std::is_same_v<ComponentType, ComponentTypes> || ...), "Invalid template parameter provided to base GetComponentHelper - component type is not managed by the queried system.");
        return _data->template GetComponentHelper<ComponentType, 0, ComponentTypes...>(componentTuple);
    }

    template<class... ComponentTypes>
    inline std::pair<bool, typename BaseComponentSystem<ComponentTypes...>::ComponentTuple> BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::FilterEntity(EntityID ID, UtilityBox::Logger::LogMessage& message) {
        ComponentTuple componentTuple;
        unsigned numMatchingComponents = 0;
        Entities::EntityManager* entityManager = ENGINE_NAME::World::GetInstance().GetEntityManager();

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

    template<class... ComponentTypes>
    BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::BaseComponentSystemData(::UtilityBox::Logger::LoggingSystem& loggingSystem, std::vector<ComponentTuple>& filteredEntities) : _loggingSystemReference(loggingSystem),
                                                                                                                                                                                                  _filteredEntitiesReference(filteredEntities)
                                                                                                                                                                                                  {
        // Register callback functions.
        Entities::EntityManager* entityManager = ENGINE_NAME::World::GetInstance().GetEntityManager();

        // Register callback for when entities get created.
        entityManager->RegisterCallback(&BaseComponentSystemData::OnEntityCreate, Entities::EntityManager::CallbackType::ENTITY_CREATE);
        // Register callback for when entities get deleted.
        entityManager->RegisterCallback(&BaseComponentSystemData::OnEntityDestroy, Entities::EntityManager::CallbackType::ENTITY_DELETE);
        // Register callback for when a component is attached to an entity.
        entityManager->RegisterCallback(&BaseComponentSystemData::OnEntityComponentAdd, Entities::EntityManager::CallbackType::COMPONENT_ADD);
        // Register callback for when a component is removed from an entity.
        entityManager->RegisterCallback(&BaseComponentSystemData::OnEntityComponentRemove, Entities::EntityManager::CallbackType::COMPONENT_REMOVE);
    }

    template<class... ComponentTypes>
    BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::~BaseComponentSystemData() {
        _containerIndexToEntityID.clear();
        _entityIDToContainerIndex.clear();
    }

    template<class... ComponentTypes>
    void BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::VerifyEntity(EntityID ID, UtilityBox::Logger::LogMessage& message) {
        // Attempt to filter entity by components.
        std::pair<bool, ComponentTuple> filterResult = FilterEntity(ID, message);

        // Passed filter.
        if (filterResult.first) {
            _filteredEntitiesReference.emplace_back(std::move(filterResult.second));

            // Set up mapping for further access.
            unsigned filteredEntityIndex = _filteredEntitiesReference.size() - 1;
            _entityIDToContainerIndex.emplace(ID, filteredEntityIndex);
            _containerIndexToEntityID.emplace(filteredEntityIndex, ID);

            message.Supply("Emplacing component tuple at position: %i.", filteredEntityIndex);
        }
    }

    template<class... ComponentTypes>
    void BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::RemoveEntity(const std::unordered_map<EntityID, unsigned>::const_iterator& entityToDeleteIterator, UtilityBox::Logger::LogMessage& message) {
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

    template<class... ComponentTypes>
    template<class DesiredComponentType, unsigned INDEX, class ComponentType, class ...AdditionalComponentArgs>
    inline DesiredComponentType* BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::GetComponentHelper(const BaseComponentSystem::ComponentTuple& componentTuple) {
        if constexpr (DesiredComponentType::ID == ComponentType::ID) {
            return std::get<INDEX>(componentTuple);
        }
        else {
            return GetComponentHelper<DesiredComponentType, INDEX + 1, AdditionalComponentArgs...>(componentTuple);
        }
    }

    template<class... ComponentTypes>
    template<unsigned int INDEX, class ComponentType, class... AdditionalComponentArgs>
    inline bool BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::ProcessEntityComponent(ComponentTypeID componentTypeID, Components::BaseComponent *component, BaseComponentSystem::ComponentTuple &componentTuple, UtilityBox::Logger::LogMessage& message) {
        const char* componentName = ComponentType::ToString();
        message.Supply("Attempting to pair component: '%s' from entity component list into component tuple.", componentName);

        // ID's match, we found a component in the entity that is included in the ComponentTuple that the system manages.
        if (ComponentType::ID == componentTypeID) {
            // We know the component exists, dynamic cast should always succeed.
            ComponentType* derivedComponentType = dynamic_cast<ComponentType*>(component);
            ASSERT(derivedComponentType != nullptr, "System failed to get matching component."); // Should never happen.

            std::get<INDEX>(componentTuple) = derivedComponentType;
            message.Supply("Pairing success - entity component '%s' added to component tuple.", componentName);

            return true;
        }
        else {
            message.Supply("Pairing failed - component types do not match. Moving on to next tuple position.");
            return ProcessEntityComponent<INDEX + 1, AdditionalComponentArgs...>(componentTypeID, component, componentTuple, message);
        }
    }

    template<class... ComponentTypes>
    template<unsigned>
    inline bool BaseComponentSystem<ComponentTypes...>::BaseComponentSystemData::ProcessEntityComponent(ComponentTypeID, Components::BaseComponent*, BaseComponentSystem::ComponentTuple&, UtilityBox::Logger::LogMessage& message) {
        // There are no more components that the system manages to go through, 'component' is not managed by this system.
        message.Supply("Component type is not managed by this system. No changes made to component tuple.");
        return false;
    }
}

#endif // BASE_COMPONENT_SYSTEM_TPP
