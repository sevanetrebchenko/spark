
#ifndef SPARK_ENTITY_EVENTS_H
#define SPARK_ENTITY_EVENTS_H

#include "spark/utility.h"
#include "spark/events/types/base_event.h"

namespace Spark {
    namespace Events {

        class EntityCreatedEvent : public IEvent {
            public:
                REGISTER_EVENT(EntityCreatedEvent);

                explicit EntityCreatedEvent(ECS::EntityID entityID);
                NODISCARD const ECS::EntityID& GetEntityID() const;

            private:
                ECS::EntityID entityID_;
        };


        class EntityDestroyedEvent : public IEvent {
            public:
                REGISTER_EVENT(EntityDestroyedEvent);

                explicit EntityDestroyedEvent(ECS::EntityID entityID);
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
