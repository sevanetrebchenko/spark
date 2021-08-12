
#ifndef SPARK_EVENT_HUB_H
#define SPARK_EVENT_HUB_H

#include "spark/utility.h"
#include "spark/tools/singleton.h"
#include "spark/events/event_listener_interface.h"

namespace Spark {
    namespace Events {

        class EventHub : Singleton<EventHub> {
            public:
                REGISTER_SINGLETON(EventHub);

                void AttachEventListener(IEventListener* eventListener);
                void DetachEventListener(IEventListener* eventListener);

                void Dispatch(const EventHandle& event);
                void OnUpdate(float dt);

            private:
                std::vector<IEventListener*> listeners_;
        };

    }
}

#define MAKE_EVENT(EventType, ...) std::make_shared<const ::Spark::Events::IEvent>(new EventType(__VA_ARGS__))

#endif // SPARK_EVENT_HUB_H
