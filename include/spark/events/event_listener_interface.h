
#ifndef SPARK_EVENT_LISTENER_INTERFACE_H
#define SPARK_EVENT_LISTENER_INTERFACE_H

#include <events/event.h>
#include <spark_pch.h>

namespace Spark {
    namespace Events {

        // Interface for making EventListeners polymorphic at runtime.
        class IEventListener {
            public:
                /**
                 * Send an event to this function. Function adequately handles if the given eventType is not handled by
                 * the attached EventListener.
                 * @param eventPointer - Shared pointer to the dispatched event.
                 */
                virtual void OnEventReceived(std::shared_ptr<Event*> eventPointer) = 0;

                virtual void OnUpdate() = 0;
        };

    }
}

#endif //SPARK_EVENT_LISTENER_INTERFACE_H
