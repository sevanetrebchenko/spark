
#ifndef SPARK_EVENT_LISTENER_INTERFACE_H
#define SPARK_EVENT_LISTENER_INTERFACE_H

#include "spark/events/event_definitions.h"

namespace Spark {
    namespace Events {

        class IEventListener {
            public:
                virtual void OnEventReceived(const EventHandle& event) = 0;
                virtual void OnUpdate(float dt) = 0;
        };

    }
}

#endif //SPARK_EVENT_LISTENER_INTERFACE_H
