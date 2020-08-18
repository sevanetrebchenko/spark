
#ifndef SPARK_EVENT_HUB_INTERFACE_H
#define SPARK_EVENT_HUB_INTERFACE_H

#include "event_listener_interface.h" // IEventListener
#include "event.h"                    // Event

namespace Spark {
    namespace Events {

        class IEventHub {
            public:
                virtual void AttachEventListener(IEventListener* eventListener) = 0;
                virtual void DetachEventListener(IEventListener* eventListener) = 0;
                virtual void Dispatch(Event* event) = 0;
                virtual void OnUpdate() = 0;
        };

    }
}

#endif //SPARK_EVENT_HUB_INTERFACE_H
