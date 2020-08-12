
#ifndef SPARK_EVENT_LISTENER_H
#define SPARK_EVENT_LISTENER_H

#include <events/event_listener_interface.h>
#include <events/event.h>

namespace Spark {
    namespace Events {

        template <class ...EventTypes>
        class EventListener : public EventListenerInterface {
            public:
                EventListener();
                ~EventListener();

                void OnEventReceived(std::shared_ptr<Event*> eventPointer) override;

            private:
                class EventListenerData;
                EventListenerData* _data;
        };

    }
}

#include "../../../src/events/event_listener.tpp"

#endif //SPARK_EVENT_LISTENER_H
