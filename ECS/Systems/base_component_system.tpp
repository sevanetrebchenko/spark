
#ifndef BASE_COMPONENT_SYSTEM_TPP
#define BASE_COMPONENT_SYSTEM_TPP

#define INVALID_INDEX _filteredEntities.size()

#include "../Components/base_component.h"
#include "ECS/Entities/entity_manager.h"

namespace ECS::Components {
    template <class... ComponentTypes>
    inline ComponentSystem<ComponentTypes...>::ComponentSystem() {
        static_assert((std::is_base_of<ECS::Components::BaseComponent, ComponentTypes>::value && ...), "Invalid template parameter provided to base ComponentSystem - component types must derive from ECS::Components::BaseComponent.");

        // Register callback functions.
        // TODO;

        // Defer initialization to second stage.
    }

    template <class... ComponentTypes>
    inline void ComponentSystem<ComponentTypes...>::Initialize() {

    }

    template <class... ComponentTypes>
    inline ComponentSystem<ComponentTypes...>::~ComponentSystem() {
        _filteredEntities.clear();
    }

    template<class... ComponentTypes>
    inline void ComponentSystem<ComponentTypes...>::OnEntityCreate(ECS::EntityID ID) {
        FilterEntity(ID);
    }

    template<class... ComponentTypes>
    inline void ComponentSystem<ComponentTypes...>::OnEntityDestroy(ECS::EntityID ID) {
        // Find the location of the component tuple in the filtered entities array that belongs to the entity.
        auto entityToDeleteIterator = _entityIDToContainerIndex.find(ID); // <EntityID, index>

        // Ensure component tuple with provided ID exists.
        if (entityToDeleteIterator != _entityIDToContainerIndex.end()) {
            unsigned toDeleteIndex = entityToDeleteIterator->second; // Index of the component tuple to delete.
            unsigned lastEntityIndex = _filteredEntities.size() - 1; // Index of the last component tuple.

            // Get the ID of the component tuple at the last index for later use.
            auto lastEntityIterator = _containerIndexToEntityID.find(lastEntityIndex); // <index, EntityID>
            EntityID lastEntityID = lastEntityIterator->second;

            // Delete component tuple.
            // Overwrite the component tuple marked for deletion with the last one and decrease container size by 1.
            _filteredEntities[toDeleteIndex] = std::move(_filteredEntities[lastEntityIndex]);
            _filteredEntities.pop_back();

            // Update index and ID maps to reflect change.
            auto IDMapIterator = _entityIDToContainerIndex.find(lastEntityID); // <EntityID, index>
            IDMapIterator->second = toDeleteIndex;
            auto indexMapIterator = _containerIndexToEntityID.find(toDeleteIndex); // <index, EntityID>
            indexMapIterator->second = lastEntityID;
        }
    }

    template<class... ComponentTypes>
    inline void ComponentSystem<ComponentTypes...>::OnEntityComponentAdd(ECS::EntityID ID) {
        // Perform entity filtering if the entity with the given ID is not already part of the system.
        if (_entityIDToContainerIndex.find(ID) == _entityIDToContainerIndex.end()) {
            FilterEntity(ID);
        }
    }

    template<class... ComponentTypes>
    inline void ComponentSystem<ComponentTypes...>::OnEntityComponentRemove(ECS::EntityID ID) {
        FilterEntity(ID);
    }

    template<class... ComponentTypes>
    void ComponentSystem<ComponentTypes...>::FilterEntity(ECS::EntityID ID) {
        ComponentTuple componentTuple;
        unsigned numMatchingComponents = 0;
        ECS::Entities::EntityManager entityManager = ECS::Entities::EntityManager(); // TODO get from one location

        for (const std::pair<ComponentTypeID, Components::BaseComponent*>& entityComponentPair : entityManager.GetComponents(ID)) {
            // Attempt to see if this component is present in the system's component list.
            if (ProcessEntityComponent<0, ComponentTypes...>(entityComponentPair.first, entityComponentPair.second, componentTuple)) {
                // Component was matched and filled in the component tuple.
                ++numMatchingComponents;

                // If the number of matching components is equal to the total number of components the system manages,
                // the component tuple is fully constructed and can be appended to the list of components.
                if (numMatchingComponents == sizeof...(ComponentTypes)) {
                    _filteredEntities.emplace_back(componentTuple);

                    // Set up mapping for further access.
                    unsigned filteredEntityIndex = _filteredEntities.size() - 1;
                    _entityIDToContainerIndex.emplace(ID, filteredEntityIndex);
                    _containerIndexToEntityID.emplace(filteredEntityIndex, ID);
                    break;
                }
            }
        }
        // Entity is not managed by this system, do nothing.
    }

    template<class... ComponentTypes>
    template<unsigned int INDEX, class ComponentType, class... AdditionalComponentArgs>
    bool ComponentSystem<ComponentTypes...>::ProcessEntityComponent(ECS::ComponentTypeID ID, ECS::Components::BaseComponent *component, ComponentSystem::ComponentTuple &componentTuple) {
        // ID's match, we found a component in the entity that is included in the ComponentTuple that the system manages.
        if (ComponentType::ID == ID) {
            // We know the component exists, dynamic cast should always succeed.
            ComponentType* derivedComponentType = dynamic_cast<ComponentType>(component);
            ASSERT(derivedComponentType != nullptr, "System failed to get matching component.");

            std::get<INDEX>(componentTuple) = derivedComponentType;
            return true;
        }
        else {
            return ProcessEntityComponent<INDEX + 1, AdditionalComponentArgs...>(ID, component, componentTuple);
        }
    }

    template<class... ComponentTypes>
    template<unsigned int INDEX>
    bool ComponentSystem<ComponentTypes...>::ProcessEntityComponent(ECS::ComponentTypeID ID, ECS::Components::BaseComponent *component, ComponentSystem::ComponentTuple &componentTuple) {
        // There are no more components that the system manages to go through, 'component' is not managed by this system.
        return false;
    }
}

#endif // BASE_COMPONENT_SYSTEM_TPP
