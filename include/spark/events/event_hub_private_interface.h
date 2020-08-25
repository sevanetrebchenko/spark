
#ifndef SPARK_EVENT_HUB_PRIVATE_INTERFACE_H
#define SPARK_EVENT_HUB_PRIVATE_INTERFACE_H

#include <spark/events/event_hub_interface.h>      // IEventHub
#include <spark/events/event_listener_interface.h> // IEventListener

namespace Spark {
    namespace Events {

        class IEventHubPrivate : public IEventHub {
            public:
                virtual void AttachEventListener(IEventListener* eventListener) = 0;
                virtual void DetachEventListener(IEventListener* eventListener) = 0;
                virtual void OnUpdate() = 0;
        };

    }
}

#endif // SPARK_EVENT_HUB_PRIVATE_INTERFACE_H
