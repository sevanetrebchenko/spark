
#ifndef SPARK_ENTITY_EVENTS_H
#define SPARK_ENTITY_EVENTS_H

#include <spark/core/rename.h>
#include <spark/events/types/event.h> // Event, EventType
#include <spark/ecs/ecs_typedefs.h>   // EntityID

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

        template <class ComponentType>
        class AddComponentEvent : public IEvent {
            public:
                static inline constexpr EventType StaticEventType = EventType::EntityComponentAdd;

                explicit AddComponentEvent(ECS::EntityID ID);

                NODISCARD std::string ToString() const override;
                NODISCARD const ECS::EntityID& GetEntityID() const;

            private:
                ECS::EntityID entityID_;
        };

        template <class ComponentType>
        class RemoveComponentEvent : public IEvent {
            public:
                static inline constexpr EventType StaticEventType = EventType::EntityComponentRemove;

                explicit RemoveComponentEvent(ECS::EntityID ID);

                NODISCARD std::string ToString() const override;
                NODISCARD const ECS::EntityID& GetEntityID() const;

            private:
                ECS::EntityID entityID_;
        };

        class AddComponentSystemConfigEvent : public IEvent {
            public:
                static inline constexpr EventType StaticEventType = EventType::SystemAddComponentConfig;

                AddComponentSystemConfigEvent(ECS::EntityID entityID, ECS::EntityComponentMap entityComponentMap);

                NODISCARD std::string ToString() const override;

                NODISCARD const ECS::EntityID& GetEntityID() const;
                NODISCARD const ECS::EntityComponentMap& GetComponentMap() const;

            private:
                ECS::EntityID entityID_;
                ECS::EntityComponentMap componentMap_;
        };

        class RemoveComponentSystemConfigEvent : public IEvent {
            public:
                static inline constexpr EventType StaticEventType = EventType::SystemRemoveComponentConfig;

                RemoveComponentSystemConfigEvent(ECS::EntityID entityID, ECS::EntityComponentMap entityComponentMap);

                NODISCARD std::string ToString() const override;

                NODISCARD const ECS::EntityID& GetEntityID() const;
                NODISCARD const ECS::EntityComponentMap& GetComponentMap() const;

            private:
                ECS::EntityID entityID_;
                ECS::EntityComponentMap componentMap_;
        };

    }
}

#include <spark/events/types/ecs_events.tpp> // Template function definitions.

#endif // SPARK_ENTITY_EVENTS_H
