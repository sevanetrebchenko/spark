
#ifndef SPARK_BASE_COMPONENT_SYSTEM_TPP
#define SPARK_BASE_COMPONENT_SYSTEM_TPP

#include <spark/core/rename.h>
#include <spark/ecs/components/types/base_component.h> // BaseComponent
#include <spark/utilitybox/tools/assert_dev.h>
#include <spark/ecs/entities/entity_manager.h>

namespace Spark::ECS {

    template <class... ComponentTypes>
    BaseComponentSystem<ComponentTypes...>::BaseComponentSystem(const std::string& systemName) : UtilityBox::Logger::ILoggable(systemName) {
        // Ensure data validity.
        static_assert((std::is_base_of_v<BaseComponent, ComponentTypes> && ...), "Invalid template parameter provided to base BaseComponentSystem - component types must derive from BaseComponent.");
        static_assert(sizeof...(ComponentTypes) > 0, "ComponentSystem must operate on at least one component type.");
    }

    template <class... ComponentTypes>
    BaseComponentSystem<ComponentTypes...>::~BaseComponentSystem() {
        indexToEntityID_.clear();
        entityIDToIndex_.clear();

        tuples_.clear();
    }

    template<class... ComponentTypes>
    void BaseComponentSystem<ComponentTypes...>::Initialize() {
    }

    template<class... ComponentTypes>
    void BaseComponentSystem<ComponentTypes...>::Update(float dt) {
    }

    template <class... ComponentTypes>
    void BaseComponentSystem<ComponentTypes...>::Shutdown() {
    }

    template <class... ComponentTypes>
    template <class ComponentType>
    ComponentType* BaseComponentSystem<ComponentTypes...>::GetComponent(unsigned index) {
        static_assert((std::is_same_v<ComponentType, ComponentTypes> || ...), "Component type is not managed by the queried system.");

        // Check for invalid index.
        if (index >= tuples_.size()) {
            LogError("Exception thrown: Index: %i provided to GetComponent is out of range.", index);
            throw std::out_of_range("Invalid index provided to GetComponent.");
        }

        ComponentType* component = GetComponentHelper<ComponentType, 0, ComponentTypes...>(tuples_[index]);
        return component;
    }

    template <class... ComponentTypes>
    template <class ComponentType>
    ComponentType* BaseComponentSystem<ComponentTypes...>::GetComponent(const BaseComponentSystem::ComponentTuple& componentTuple) {
        // Ensure component is managed by this system.
        static_assert((std::is_same_v<ComponentType, ComponentTypes> || ...), "Component type is not managed by the queried system.");
        return GetComponentHelper<ComponentType, 0, ComponentTypes...>(componentTuple);
    }

    template<class... ComponentTypes>
    void BaseComponentSystem<ComponentTypes...>::OnEvent(Events::RefreshObjectComponentListEvent* event) {
        ECS::EntityID entityID = event->GetEntityID();
        const EntityComponentMap* entityComponents = Singleton<EntityManager>::GetInstance()->GetEntityComponentMap(entityID);

        // Remove entity if it already exists.
        auto tupleLocationIter = entityIDToIndex_.find(entityID);
        if (tupleLocationIter != entityIDToIndex_.end()) {
            RemoveEntity(entityID);
        }

        // (Re)-process entity.
        ComponentTuple tuple { };
        bool validEntity = FilterEntity(*entityComponents, tuple);
        if (validEntity) {
            InsertTuple(entityID, tuple);
        }
    }

    template<class... ComponentTypes>
    void BaseComponentSystem<ComponentTypes...>::OnEvent(Events::DestroyEntityEvent* event) {
        ECS::EntityID entityID = event->GetEntityID();

        auto tupleLocationIter = entityIDToIndex_.find(entityID);
        if (tupleLocationIter == entityIDToIndex_.end()) {
            // Don't remove entity that doesn't exist.
            return;
        }

        RemoveEntity(entityID);
    }

    template<class... ComponentTypes>
    template<class DesiredComponentType, unsigned Index, class ComponentType, class ...AdditionalComponentArgs>
    DesiredComponentType* BaseComponentSystem<ComponentTypes...>::GetComponentHelper(const BaseComponentSystem::ComponentTuple& componentTuple) const {
        if constexpr (DesiredComponentType::ID == ComponentType::ID) {
            return std::get<Index>(componentTuple);
        }
        else {
            return GetComponentHelper<DesiredComponentType, Index + 1, AdditionalComponentArgs...>(componentTuple);
        }
    }

    template<class... ComponentTypes>
    template<class DesiredComponentType, unsigned Index>
    DesiredComponentType* BaseComponentSystem<ComponentTypes...>::GetComponentHelper(const BaseComponentSystem::ComponentTuple& componentTuple) const {
        LogError("Requesting component that isn't managed by the component system. (component type: '%s', component system components: '%s'", DesiredComponentType::Name, Internal::CommaSeparatedList<ComponentTypes...>().c_str());
        return nullptr;
    }

    template<class... ComponentTypes>
    bool BaseComponentSystem<ComponentTypes...>::FilterEntity(const EntityComponentMap& entityComponents, BaseComponentSystem::ComponentTuple &tuple) const {
        unsigned numMatching = 0;

        for (std::pair<EntityID, BaseComponent*> componentData : entityComponents) {
            ComponentTypeID componentTypeID = componentData.first;
            BaseComponent* component = componentData.second;

            if (ProcessEntityComponent<0, ComponentTypes...>(componentTypeID, component, tuple)) {
                ++numMatching;

                if (numMatching == sizeof...(ComponentTypes)) {
                    // Got all the necessary components required for this system.
                    return true;
                }
            }
        }

        return false;
    }

    template<class... ComponentTypes>
    void BaseComponentSystem<ComponentTypes...>::RemoveEntity(EntityID entityID) {
        auto tupleLocationIter = entityIDToIndex_.find(entityID);
        if (tupleLocationIter == entityIDToIndex_.end()) {
            // Don't remove entities that don't exist.
            return;
        }

        // Remove mapping for this entity.
        unsigned entityIndex = tupleLocationIter->second;

        entityIDToIndex_.erase(entityID);
        indexToEntityID_.erase(entityIndex);

        // Erase mapping for last element.
        unsigned lastIndex = tuples_.size() - 1;
        SP_ASSERT(indexToEntityID_.find(lastIndex) != indexToEntityID_.end(), "Mappings for Entity ID to tuple index are configured incorrectly.");
        EntityID lastEntityID = indexToEntityID_[lastIndex];

        entityIDToIndex_.erase(lastEntityID);
        indexToEntityID_.erase(lastIndex);

        // Swap element to delete with last element.
        std::swap(tuples_[entityIndex], tuples_[lastIndex]);
        tuples_.pop_back();

        // Update mapping of swapped element.
        entityIDToIndex_.insert(lastEntityID, entityIndex);
        indexToEntityID_.insert(entityIndex, lastEntityID);
    }

    template<class... ComponentTypes>
    void BaseComponentSystem<ComponentTypes...>::InsertTuple(EntityID entityID, ComponentTuple &tuple) {
        tuples_.emplace_back(std::move(tuple));
        unsigned index = tuples_.size();

        // Register tuple with maps.
        entityIDToIndex_.insert(entityID, index);
        indexToEntityID_.insert(index, entityID);
    }

    template<class... ComponentTypes>
    template<unsigned Index, class ComponentType, class... AdditionalComponentArgs>
    bool BaseComponentSystem<ComponentTypes...>::ProcessEntityComponent(ComponentTypeID componentTypeID, BaseComponent *component, ComponentTuple &componentTuple) {
        if (ComponentType::ID == componentTypeID) {
            ComponentType* derivedComponentType = dynamic_cast<ComponentType*>(component);
            SP_ASSERT(derivedComponentType != nullptr, "System failed to get matching component."); // Should never happen.
            SP_ASSERT(std::get<Index>(componentTuple) == nullptr, "Component has already been emplaced in system ComponentTuple.");

            // Emplace in the tuple.
            std::get<Index>(componentTuple) = derivedComponentType;
            return true;
        }
        else {
            return ProcessEntityComponent<Index + 1, AdditionalComponentArgs...>(componentTypeID, component, componentTuple);
        }
    }

    template<class... ComponentTypes>
    template<unsigned>
    bool BaseComponentSystem<ComponentTypes...>::ProcessEntityComponent(ComponentTypeID, BaseComponent*, ComponentTuple&) {
        // Component is not managed by this system (doesn't exist in the tuple).
        return false;
    }

}

#endif // SPARK_BASE_COMPONENT_SYSTEM_TPP
