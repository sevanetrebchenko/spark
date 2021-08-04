
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

    // RefreshObjectComponentListEvent
    RefreshObjectComponentListEvent::RefreshObjectComponentListEvent(ECS::EntityID ID) : IEvent(EventType::SystemRefreshObjectComponentList, EventCategory::EventCategoryECS),
                                                                                         entityID_(ID)
                                                                                         {
    }

    std::string RefreshObjectComponentListEvent::ToString() const {
        return std::string(); // TODO
    }

    const ECS::EntityID &RefreshObjectComponentListEvent::GetEntityID() const {
        return entityID_;
    }
}
