
#ifndef SPARK_ENTITY_EVENTS_H
#define SPARK_ENTITY_EVENTS_H

#include "spark/events/types/base_event.h"
#include "spark/ecs/utility.h"
#include "spark/events/utility.h"

namespace Spark {
    namespace Events {

        class EntityCreatedEvent : public IEvent {
            public:
                REGISTER_EVENT(EntityCreatedEvent);
                ~EntityCreatedEvent() override {
                    std::cout << "Entity created event destructor" << std::endl;
                }

                explicit EntityCreatedEvent(ECS::EntityID entityID);
                NODISCARD const ECS::EntityID& GetEntityID() const;

            private:
                ECS::EntityID entityID_;
        };


        class EntityDestroyedEvent : public IEvent {
            public:
                REGISTER_EVENT(EntityDestroyedEvent);
                ~EntityDestroyedEvent() override {
                    std::cout << "Entity destroyed event destructor" << std::endl;
                }

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
