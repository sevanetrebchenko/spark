
#ifndef SPARK_EVENT_H
#define SPARK_EVENT_H

#include "spark/events/event_definitions.h"

namespace Spark {
    namespace Events {

        class IEvent {
            public:
                virtual ~IEvent() = default;
                NODISCARD virtual EventTypeID GetID() const = 0;
        };

    }
}

#endif // SPARK_EVENT_H
