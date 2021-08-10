
#ifndef SPARK_EVENT_LISTENER_H
#define SPARK_EVENT_LISTENER_H

#include "spark/core/utility.h"
#include "spark/events/utility.h"
#include "spark/events/event_listener_interface.h"
#include "spark/events/types/base_event.h"
#include "spark/events/event_hub.h"

namespace Spark {
    namespace Events {

        template <class CRTP, class... EventTypes>
        class EventListener : public IEventListener,
                              Spark::Internal::RequireUniqueTypes<EventTypes...>,
                              Spark::Events::Internal::RequireOnEventForTypes<EventTypes...>
                              {
            public:
                EventListener();
                ~EventListener();

            private:
                void OnEventReceived(std::shared_ptr<const IEvent*> eventPtr) override;
                void OnUpdate(float dt) override;

                // Determine if system manages given type of event.
                template <class EventType1, class EventType2, class ...AdditionalEventTypes>
                NODISCARD bool ManagesEventType(const EventTypeID& eventType);

                template <class EventType>
                NODISCARD bool ManagesEventType(const EventTypeID& eventType);

                // Get the derived class of the base event pointer.
                template <class EventType1, class EventType2, class ...AdditionalEventTypes>
                const EventType1* GetEventOfType(const IEvent* baseEvent);

                template <class EventType>
                const EventType* GetEventOfType(const IEvent* baseEvent);

                std::queue<std::shared_ptr<const IEvent*>> eventQueue_; // Events received this frame.
        };

    }
}

#include "spark/events/event_listener.tpp"

#endif // SPARK_EVENT_LISTENER_H
