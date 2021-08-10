
#ifndef SPARK_EVENT_HUB_H
#define SPARK_EVENT_HUB_H

#include "spark/core/utility.h"
#include "spark/utilitybox/tools/singleton.h"
#include "spark/events/event_listener_interface.h"

namespace Spark {
    namespace Events {

        class EventHub : Singleton<EventHub> {
            public:
                REGISTER_SINGLETON(EventHub);

                void AttachEventListener(IEventListener* eventListener);
                void DetachEventListener(IEventListener* eventListener);

                void Dispatch(IEvent* event);
                void OnUpdate(float dt);

            private:
                std::vector<IEventListener*> listeners_;
        };

    }
}

#endif // SPARK_EVENT_HUB_H
