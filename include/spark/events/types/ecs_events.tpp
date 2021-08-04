
#ifndef SPARK_ECS_EVENTS_TPP
#define SPARK_ECS_EVENTS_TPP

namespace Spark::Events {

    // AddComponentEvent
    template<class ComponentType>
    AddComponentEvent<ComponentType>::AddComponentEvent(ECS::EntityID ID) : IEvent(EventType::EntityComponentAdd, EventCategory::EventCategoryECS),
                                                                            entityID_(ID)
                                                                            {
    }

    template<class ComponentType>
    std::string AddComponentEvent<ComponentType>::ToString() const {
        return std::string(); // TODO
    }

    template<class ComponentType>
    const ECS::EntityID &AddComponentEvent<ComponentType>::GetEntityID() const {
        return entityID_;
    }

    // RemoveComponentEvent
    template<class ComponentType>
    RemoveComponentEvent<ComponentType>::RemoveComponentEvent(ECS::EntityID ID) : IEvent(EventType::EntityComponentRemove, EventCategory::EventCategoryECS),
                                                                                  entityID_(ID)
                                                                                  {
    }

    template<class ComponentType>
    std::string RemoveComponentEvent<ComponentType>::ToString() const {
        return std::string(); // TODO
    }

    template<class ComponentType>
    const ECS::EntityID &RemoveComponentEvent<ComponentType>::GetEntityID() const {
        return entityID_;
    }

}

#endif // SPARK_ECS_EVENTS_TPP
