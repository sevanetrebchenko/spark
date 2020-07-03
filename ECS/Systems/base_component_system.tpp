
#ifndef BASE_COMPONENT_SYSTEM_TPP
#define BASE_COMPONENT_SYSTEM_TPP

#include "../Components/base_component.h"
#include "../Entities/entity_manager.h"

namespace ECS::Components {
    template <class... ComponentTypes>
    inline BaseComponentSystem<ComponentTypes...>::BaseComponentSystem(std::string&& systemName) : _systemName(std::move(systemName)) {
        static_assert((std::is_base_of<BaseComponent, ComponentTypes>::value && ...), "Invalid template parameter provided to base BaseComponentSystem - component types must derive from ECS::Components::BaseComponent.");

        // Register callback functions.
        // TODO;

        // Defer initialization to second stage.
    }

    template <class... ComponentTypes>
    inline void BaseComponentSystem<ComponentTypes...>::Initialize() {
    }

    template <class... ComponentTypes>
    inline void BaseComponentSystem<ComponentTypes...>::Shutdown() {
        _filteredEntities.clear();
        _entityIDToContainerIndex.clear();
        _containerIndexToEntityID.clear();
    }

    template<class... ComponentTypes>
    inline void BaseComponentSystem<ComponentTypes...>::OnEntityCreate(EntityID ID) {
        std::pair<bool, ComponentTuple> filterResult = FilterEntity(ID);
        if (filterResult.first) {
            _filteredEntities.emplace_back(std::move(filterResult.second));

            // Set up mapping for further access.
            unsigned filteredEntityIndex = _filteredEntities.size() - 1;
            _entityIDToContainerIndex.emplace(ID, filteredEntityIndex);
            _containerIndexToEntityID.emplace(filteredEntityIndex, ID);
        }
    }

    template<class... ComponentTypes>
    inline void BaseComponentSystem<ComponentTypes...>::OnEntityDestroy(EntityID ID) {
        // Find the location of the component tuple in the filtered entities array that belongs to the entity.
        auto entityToDeleteIterator = _entityIDToContainerIndex.find(ID); // EntityID, index

        // Ensure component tuple with provided ID exists.
        if (entityToDeleteIterator != _entityIDToContainerIndex.end()) {
            unsigned toDeleteIndex = entityToDeleteIterator->second; // Index of the component tuple to delete.
            unsigned lastEntityIndex = _filteredEntities.size() - 1; // Index of the last component tuple.

            // Get the ID of the component tuple at the last index for later use.
            auto lastEntityIterator = _containerIndexToEntityID.find(lastEntityIndex); // index, EntityID
            EntityID lastEntityID = lastEntityIterator->second;

            // Delete component tuple.
            // Overwrite the component tuple marked for deletion with the last one and decrease container size by 1.
            _filteredEntities[toDeleteIndex] = std::move(_filteredEntities[lastEntityIndex]);
            _filteredEntities.pop_back();

            // Clear index and ID from respective maps.
            _containerIndexToEntityID.erase(lastEntityIndex); // Array is shrinking from the back, last index is invalid.
            _entityIDToContainerIndex.erase(ID); // Tuple with entity ID 'ID' no longer exists.

            // Update index and ID maps to reflect change.
            auto IDMapIterator = _entityIDToContainerIndex.find(lastEntityID); // EntityID, index
            IDMapIterator->second = toDeleteIndex;
            auto indexMapIterator = _containerIndexToEntityID.find(toDeleteIndex); // index, EntityID
            indexMapIterator->second = lastEntityID;
        }
    }

    template<class... ComponentTypes>
    inline void BaseComponentSystem<ComponentTypes...>::OnEntityComponentAdd(EntityID ID) {
        // Perform entity filtering if the entity with the given ID is not already part of the system.
        if (_entityIDToContainerIndex.find(ID) == _entityIDToContainerIndex.end()) {
            std::pair<bool, ComponentTuple> filterResult = FilterEntity(ID);
            if (filterResult.first) {
                _filteredEntities.emplace_back(std::move(filterResult.second));

                // Set up mapping for further access.
                unsigned filteredEntityIndex = _filteredEntities.size() - 1;
                _entityIDToContainerIndex.emplace(ID, filteredEntityIndex);
                _containerIndexToEntityID.emplace(filteredEntityIndex, ID);
            }
        }
    }

    template<class... ComponentTypes>
    inline void BaseComponentSystem<ComponentTypes...>::OnEntityComponentRemove(EntityID ID) {
        // Ensure component tuple with provided ID exists in the managed component tuples of the system.
        if (_entityIDToContainerIndex.find(ID) != _entityIDToContainerIndex.end()) {
            // Filter entity.
            std::pair<bool, ComponentTuple> filterResult = FilterEntity(ID);

            // If the entity fails to pass filtering, remove it from the system.
            if (!filterResult.first) {
                OnEntityDestroy(ID);
            }
        }
    }

    template <class... ComponentTypes>
    template <class ComponentType>
    inline ComponentType* BaseComponentSystem<ComponentTypes...>::GetComponent(unsigned index) {
        if (index >= _filteredEntities.size()) {
            throw std::out_of_range("Invalid index provided to GetComponent.");
        }

        return GetComponentHelper<ComponentType, 0, ComponentTypes...>(_filteredEntities[index]);
    }

    template <class... ComponentTypes>
    template <class ComponentType>
    inline ComponentType* BaseComponentSystem<ComponentTypes...>::GetComponent(const BaseComponentSystem::ComponentTuple& componentTuple) {
        return GetComponentHelper<ComponentType, 0, ComponentTypes...>(componentTuple);
    }

    template<class... ComponentTypes>
    inline std::pair<bool, typename BaseComponentSystem<ComponentTypes...>::ComponentTuple> BaseComponentSystem<ComponentTypes...>::FilterEntity(EntityID ID) {
        ComponentTuple componentTuple;
        unsigned numMatchingComponents = 0;
        Entities::EntityManager entityManager = Entities::EntityManager(); // TODO get from one location

        for (const auto& entityComponentPair : entityManager.GetComponents(ID)) { // ComponentTypeID, BaseComponent*
            // Attempt to see if this component is present in the system's component list.
            if (ProcessEntityComponent<0, ComponentTypes...>(entityComponentPair.first, entityComponentPair.second, componentTuple)) {
                // Component was matched and filled in the component tuple.
                ++numMatchingComponents;

                // If the number of matching components is equal to the total number of components the system manages,
                // the component tuple is fully constructed and can be appended to the list of components.
                if (numMatchingComponents == sizeof...(ComponentTypes)) {
                    return std::make_pair(true, std::move(componentTuple));
                }
            }
        }
        // Entity is not managed by this system.
        return std::make_pair(false, std::move(componentTuple));
    }

    template<class... ComponentTypes>
    template<class DesiredComponentType, unsigned INDEX, class ComponentType, class ...AdditionalComponentArgs>
    inline DesiredComponentType* BaseComponentSystem<ComponentTypes...>::GetComponentHelper(const BaseComponentSystem::ComponentTuple& componentTuple) {
//        std::cout << "index: " << INDEX << ", desired: " << DesiredComponentType::ID << ", current: " << ComponentType::ID << std::endl;
        if constexpr (DesiredComponentType::ID == ComponentType::ID) {
            return std::get<INDEX>(componentTuple);
        }
        else {
            return GetComponentHelper<DesiredComponentType, INDEX + 1, AdditionalComponentArgs...>(componentTuple);
        }
    }

    template<class... ComponentTypes>
    template<class DesiredComponentType, unsigned>
    inline DesiredComponentType* BaseComponentSystem<ComponentTypes...>::GetComponentHelper(const BaseComponentSystem::ComponentTuple& componentTuple) {
        return nullptr;
    }

    template<class... ComponentTypes>
    template<unsigned int INDEX, class ComponentType, class... AdditionalComponentArgs>
    inline bool BaseComponentSystem<ComponentTypes...>::ProcessEntityComponent(ComponentTypeID componentTypeID, BaseComponent *component, BaseComponentSystem::ComponentTuple &componentTuple) {
        // ID's match, we found a component in the entity that is included in the ComponentTuple that the system manages.
        if constexpr (ComponentType::ID == componentTypeID) {
            // We know the component exists, dynamic cast should always succeed.
            ComponentType* derivedComponentType = dynamic_cast<ComponentType>(component);
            ASSERT(derivedComponentType != nullptr, "System failed to get matching component."); // Should never happen.

            std::get<INDEX>(componentTuple) = derivedComponentType;
            return true;
        }
        else {
            return ProcessEntityComponent<INDEX + 1, AdditionalComponentArgs...>(componentTypeID, component, componentTuple);
        }
    }

    template<class... ComponentTypes>
    template<unsigned>
    inline bool BaseComponentSystem<ComponentTypes...>::ProcessEntityComponent(ComponentTypeID, BaseComponent*, BaseComponentSystem::ComponentTuple&) {
        // There are no more components that the system manages to go through, 'component' is not managed by this system.
        return false;
    }
}

#endif // BASE_COMPONENT_SYSTEM_TPP
