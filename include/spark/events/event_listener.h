
#ifndef SPARK_EVENT_LISTENER_H
#define SPARK_EVENT_LISTENER_H

#include "spark/utility.h"
#include "spark/events/event_listener_interface.h"
#include "spark/events/types/base_event.h"
#include "spark/events/event_hub.h"

namespace Spark {
    namespace Events {

        template <class CRTP, class... EventTypes>
        class EventListener : public IEventListener,
                              ::Spark::Internal::RequireUniqueTypes<EventTypes...>,
                              Internal::RequireOnEventForTypes<EventTypes...>
                              {
            public:
                EventListener();
                ~EventListener();

            private:
                void OnEventReceived(const EventHandle& eventPtr) override;
                void OnUpdate(float dt) override;

                // Determine if system manages given type of event.
                template <class EventType1, class EventType2, class ...AdditionalEventTypes>
                NODISCARD bool ManagesEventType(const EventTypeID& eventType);

                template <class EventType>
                NODISCARD bool ManagesEventType(const EventTypeID& eventType);

                // Calls OnEvent with the derived class event pointer.
                template <class EventType1, class EventType2, class ...AdditionalEventTypes>
                void DispatchEvent(const EventHandle& event);

                template <class EventType>
                void DispatchEvent(const EventHandle& event);

                std::queue<EventHandle> eventQueue_; // Events received this frame.
        };

    }
}

#include "spark/events/event_listener.tpp"

#endif // SPARK_EVENT_LISTENER_H
