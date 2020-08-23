
#ifndef SPARK_ECS_EVENTS_TPP
#define SPARK_ECS_EVENTS_TPP

#include <spark/core/service_locator.h>    // ServiceLocator

namespace Spark::Events {
    template<class ComponentType>
    AddComponentEvent<ComponentType>::AddComponentEvent(ECS::EntityID ID) : Event(EventType::EntityComponentAdd, EventCategory::EventCategoryECS),
                                                                            _entityID(ID) {
        // Nothing to do here.
    }

    template<class ComponentType>
    AddComponentEvent<ComponentType>::AddComponentEvent(const std::string& name) : Event(EventType::EntityComponentAdd, EventCategory::EventCategoryECS),
                                                                                   _entityID(ServiceLocator::GetEntityManager()->GetEntityIDFromName(name)) {
        // Nothing to do here.
    }

    template<class ComponentType>
    std::string AddComponentEvent<ComponentType>::ToString() const {
        return "[AddComponent]";
    }

    template<class ComponentType>
    const ECS::EntityID &AddComponentEvent<ComponentType>::GetEntityID() const {
        return _entityID;
    }

    template<class ComponentType>
    RemoveComponentEvent<ComponentType>::RemoveComponentEvent(ECS::EntityID ID) : Event(EventType::EntityComponentRemove, EventCategory::EventCategoryECS),
                                                                                  _entityID(ID) {
        // Nothing to do here.
    }

    template<class ComponentType>
    RemoveComponentEvent<ComponentType>::RemoveComponentEvent(const std::string& name) : Event(EventType::EntityComponentAdd, EventCategory::EventCategoryECS),
                                                                                         _entityID(ServiceLocator::GetEntityManager()->GetEntityIDFromName(name)) {
        // Nothing to do here.
    }

    template<class ComponentType>
    std::string RemoveComponentEvent<ComponentType>::ToString() const {
        return "[RemoveComponent]";
    }

    template<class ComponentType>
    const ECS::EntityID &RemoveComponentEvent<ComponentType>::GetEntityID() const {
        return _entityID;
    }
}

#endif // SPARK_ECS_EVENTS_TPP
