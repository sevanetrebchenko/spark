
#ifndef SPARK_ENTITY_EVENTS_H
#define SPARK_ENTITY_EVENTS_H

#include <events/event.h>
#include <ecs/ecs_typedefs.h>

namespace Spark {
    namespace ECS {
        namespace Entities {

            template <class ComponentType>
            class AddComponentEvent : public Events::Event {
                public:
                    static inline constexpr Events::EventType GetStaticEventType() { return Events::EventType::KeyPressed; }

                    AddComponentEvent();
                    explicit AddComponentEvent(EntityID ID);
                    explicit AddComponentEvent(std::string name);

                    virtual std::string ToString() const override;

                    const EntityID& GetEntityID() const;
                    const std::string& GetEntityName() const;

                private:
                    EntityID _entityID;
                    std::string _entityName;
            };

            template <class ComponentType>
            class RemoveComponentEvent : public Events::Event {
                public:
                    static inline constexpr Events::EventType GetStaticEventType() { return Events::EventType::KeyPressed; }
                    explicit RemoveComponentEvent(EntityID ID);
                    explicit RemoveComponentEvent(std::string name);

                    const EntityID& GetEntityID() const;
                    const std::string& GetEntityName() const;

                private:
                    EntityID _entityID;
                    std::string _entityName;
            };

        }
    }
}

#include "../../../src/ecs/entities/entity_events.tpp" // Template function definitions.

#endif //SPARK_ENTITY_EVENTS_H
