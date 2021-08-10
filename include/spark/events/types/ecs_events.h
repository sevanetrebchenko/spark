
#ifndef SPARK_ENTITY_EVENTS_H
#define SPARK_ENTITY_EVENTS_H

#include "spark/events/types/base_event.h"
#include "spark/ecs/utility.h"
#include "spark/events/utility.h"

namespace Spark {
    namespace Events {

        class CreateEntityEvent : public IEvent {
            public:
                REGISTER_EVENT(CreateEntityEvent);

                explicit CreateEntityEvent(ECS::EntityID entityID);
                NODISCARD const ECS::EntityID& GetEntityID() const;

            private:
                ECS::EntityID entityID_;
        };


        class DestroyEntityEvent : public IEvent {
            public:
                REGISTER_EVENT(DestroyEntityEvent);

                explicit DestroyEntityEvent(ECS::EntityID entityID);
                NODISCARD const ECS::EntityID& GetEntityID() const;

            private:
                ECS::EntityID entityID_;
        };


        class RefreshObjectComponentListEvent : public IEvent {
            public:
                REGISTER_EVENT(RefreshObjectComponentListEvent);

                explicit RefreshObjectComponentListEvent(ECS::EntityID ID);
                NODISCARD const ECS::EntityID& GetEntityID() const;

            private:
                ECS::EntityID entityID_;
        };

    }
}

#endif // SPARK_ENTITY_EVENTS_H
