
#ifndef SPARK_ENTITY_EVENTS_H
#define SPARK_ENTITY_EVENTS_H

#include <spark_pch.h>                             // std::string
#include <spark/events//types/event.h>             // Event, EventType
#include <spark/ecs/ecs_typedefs.h>                // EntityID
#include <spark/utilitybox/tools/global_defines.h> // _NODISCARD_

namespace Spark {
    namespace Events {

        template <class ComponentType>
        class AddComponentEvent : public Event {
            public:
                static inline constexpr EventType StaticEventType = EventType::EntityComponentAdd;

                explicit AddComponentEvent(ECS::EntityID ID);
                explicit AddComponentEvent(const std::string& name);

                _NODISCARD_ std::string ToString() const override;

                _NODISCARD_ const ECS::EntityID& GetEntityID() const;

            private:
                ECS::EntityID _entityID;
        };

        template <class ComponentType>
        class RemoveComponentEvent : public Events::Event {
            public:
                static inline constexpr EventType StaticEventType = EventType::EntityComponentRemove;

                explicit RemoveComponentEvent(ECS::EntityID ID);
                explicit RemoveComponentEvent(const std::string& name);

                _NODISCARD_ std::string ToString() const override;

                _NODISCARD_ const ECS::EntityID& GetEntityID() const;

            private:
                ECS::EntityID _entityID;
        };

    }
}

#include <events/types/ecs_events.tpp> // Template function definitions.

#endif // SPARK_ENTITY_EVENTS_H
