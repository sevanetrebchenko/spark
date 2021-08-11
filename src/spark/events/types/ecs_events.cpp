
#include "spark/events/types/ecs_events.h"

namespace Spark::Events {

    // CreateEntityEvent.
    EntityCreatedEvent::EntityCreatedEvent(ECS::EntityID entityID) : entityID_(entityID) {
    }

    const ECS::EntityID &EntityCreatedEvent::GetEntityID() const {
        return entityID_;
    }


    // DestroyEntityEvent
    EntityDestroyedEvent::EntityDestroyedEvent(ECS::EntityID entityID) : entityID_(entityID) {
    }

    const ECS::EntityID &EntityDestroyedEvent::GetEntityID() const {
        return entityID_;
    }


    // RefreshObjectComponentListEvent
    RefreshObjectComponentListEvent::RefreshObjectComponentListEvent(ECS::EntityID entityID) : entityID_(entityID) {
    }

    const ECS::EntityID &RefreshObjectComponentListEvent::GetEntityID() const {
        return entityID_;
    }

}
