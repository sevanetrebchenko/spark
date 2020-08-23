
#ifndef SPARK_EVENT_HUB_INTERFACE_H
#define SPARK_EVENT_HUB_INTERFACE_H

#include <spark/events/types/event.h> // Event

namespace Spark {
    namespace Events {

        class IEventHub {
            public:
                virtual void Dispatch(Event* event) = 0;
        };

    }
}

#endif // SPARK_EVENT_HUB_INTERFACE_H
