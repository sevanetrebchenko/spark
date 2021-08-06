
#ifndef SPARK_ENTITY_EVENTS_H
#define SPARK_ENTITY_EVENTS_H

#include <spark/events/types/event.h>
#include <spark/ecs/utility.h>

namespace Spark {
    namespace Events {

        class CreateEntityEvent : public IEvent {
            public:
                static inline constexpr EventType StaticEventType = EventType::EntityCreate;

                explicit CreateEntityEvent(ECS::EntityID entityID);

                NODISCARD std::string ToString() const override;
                NODISCARD const ECS::EntityID& GetEntityID() const;

            private:
                ECS::EntityID entityID_;
        };

        class DestroyEntityEvent : public IEvent {
            public:
                static inline constexpr EventType StaticEventType = EventType::EntityDestroy;

                explicit DestroyEntityEvent(ECS::EntityID entityID);

                NODISCARD std::string ToString() const override;
                NODISCARD const ECS::EntityID& GetEntityID() const;

            private:
                ECS::EntityID entityID_;
        };

        class RefreshObjectComponentListEvent : public IEvent {
            public:
                static inline constexpr EventType StaticEventType = EventType::SystemRefreshObjectComponentList;

                explicit RefreshObjectComponentListEvent(ECS::EntityID ID);

                NODISCARD std::string ToString() const override;
                NODISCARD const ECS::EntityID& GetEntityID() const;

            private:
                ECS::EntityID entityID_;
        };

    }
}

#endif // SPARK_ENTITY_EVENTS_H
