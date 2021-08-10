
#include "spark/events/types/ecs_events.h"

namespace Spark::Events {

    // CreateEntityEvent.
    CreateEntityEvent::CreateEntityEvent(ECS::EntityID entityID) : entityID_(entityID) {
    }

    const ECS::EntityID &CreateEntityEvent::GetEntityID() const {
        return entityID_;
    }


    // DestroyEntityEvent
    DestroyEntityEvent::DestroyEntityEvent(ECS::EntityID entityID) : entityID_(entityID) {
    }

    const ECS::EntityID &DestroyEntityEvent::GetEntityID() const {
        return entityID_;
    }


    // RefreshObjectComponentListEvent
    RefreshObjectComponentListEvent::RefreshObjectComponentListEvent(ECS::EntityID ID) : entityID_(ID) {
    }

    const ECS::EntityID &RefreshObjectComponentListEvent::GetEntityID() const {
        return entityID_;
    }

}
