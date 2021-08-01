
#include <spark/events/types/ecs_events.h>

namespace Spark::Events {

    // CreateEntityEvent
    CreateEntityEvent::CreateEntityEvent(ECS::EntityID entityID) : IEvent(EventType::EntityCreate, EventCategory::EventCategoryECS),
                                                                   entityID_(entityID)
                                                                   {
    }

    std::string CreateEntityEvent::ToString() const {
        return std::string(); // TODO
    }

    const ECS::EntityID &CreateEntityEvent::GetEntityID() const {
        return entityID_;
    }


    // DestroyEntityEvent
    DestroyEntityEvent::DestroyEntityEvent(ECS::EntityID entityID) : IEvent(EventType::EntityDestroy, EventCategory::EventCategoryECS),
                                                                     entityID_(entityID)
                                                                     {
    }

    std::string DestroyEntityEvent::ToString() const {
        return std::string(); // TODO
    }

    const ECS::EntityID &DestroyEntityEvent::GetEntityID() const {
        return entityID_;
    }


    // AddComponentSystemConfigEvent
    AddComponentSystemConfigEvent::AddComponentSystemConfigEvent(ECS::EntityID entityID, ECS::EntityComponentMap entityComponentMap) : IEvent(EventType::SystemAddComponentConfig, EventCategory::EventCategoryECS),
                                                                                                                                       entityID_(entityID),
                                                                                                                                       componentMap_(std::move(entityComponentMap))
                                                                                                                                       {
    }

    std::string AddComponentSystemConfigEvent::ToString() const {
        return std::string(); // TODO
    }

    const ECS::EntityID &AddComponentSystemConfigEvent::GetEntityID() const {
        return entityID_;
    }

    const ECS::EntityComponentMap &AddComponentSystemConfigEvent::GetComponentMap() const {
        return componentMap_;
    }


    // RemoveComponentSystemConfigEvent
    RemoveComponentSystemConfigEvent::RemoveComponentSystemConfigEvent(ECS::EntityID entityID, ECS::EntityComponentMap entityComponentMap) : IEvent(EventType::SystemRemoveComponentConfig, EventCategory::EventCategoryECS),
                                                                                                                                             entityID_(entityID),
                                                                                                                                             componentMap_(std::move(entityComponentMap))
                                                                                                                                             {
    }

    std::string RemoveComponentSystemConfigEvent::ToString() const {
        return std::string(); // TODO
    }

    const ECS::EntityID &RemoveComponentSystemConfigEvent::GetEntityID() const {
        return entityID_;
    }

    const ECS::EntityComponentMap &RemoveComponentSystemConfigEvent::GetComponentMap() const {
        return componentMap_;
    }

}
