
#ifndef SPARK_EVENT_LISTENER_H
#define SPARK_EVENT_LISTENER_H

#include <spark_pch.h>                       // std::shared_ptr, std::queue, std::function
#include <spark/events/types/event.h>        // Event
#include <events/event_listener_interface.h> // IEventListener

namespace Spark {
    namespace Events {

        // Template parameters for EventListener registration must derive from base Event class.
        template <class ...EventTypes>
        class EventListener : public IEventListener {
            public:
                explicit EventListener(const std::function<void(std::queue<std::shared_ptr<Event*>>&)>& eventProcessingFunction);
                ~EventListener();

                bool OnEventReceived(std::shared_ptr<Event*> eventPointer) override;

            private:
                void OnUpdate() override;

                class EventListenerData;
                EventListenerData* _data;
        };

    }
}

#include <events/event_listener.tpp> // Template function definitions.

#endif // SPARK_EVENT_LISTENER_H
