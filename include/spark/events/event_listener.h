
#ifndef SPARK_EVENT_LISTENER_H
#define SPARK_EVENT_LISTENER_H

#include <events/event_listener_interface.h>
#include <events/event.h>

namespace Spark {
    namespace Events {

        // Template parameters for EventListener registration must derive from base Event class.
        template <class ...EventTypes>
        class EventListener : public IEventListener {
            public:
                explicit EventListener(std::function<void(std::queue<std::shared_ptr<Event*>>&)> eventProcessingFunction);
                ~EventListener();

                void OnEventReceived(std::shared_ptr<Event*> eventPointer) override;

            private:
                void OnUpdate() override;

                class EventListenerData;
                EventListenerData* _data;
        };

    }
}

#include "../../../src/events/event_listener.tpp"

#endif //SPARK_EVENT_LISTENER_H
