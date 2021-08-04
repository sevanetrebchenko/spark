
#ifndef SPARK_EVENT_HUB_H
#define SPARK_EVENT_HUB_H

#include <spark/events/event_listener_interface.h>    // IEventListener
#include <spark/utilitybox/tools/singleton.h>

namespace Spark {
    namespace Events {

            class EventHub : Singleton<EventHub> {
                public:
                    REGISTER_SINGLETON(EventHub);

                    void AttachEventListener(IEventListener* eventListener);
                    void DetachEventListener(IEventListener* eventListener);
                    void Dispatch(IEvent* event);
                    void OnUpdate();

                private:
                    std::vector<IEventListener*> _eventListeners;
            };

    }
}

#endif // SPARK_EVENT_HUB_H
