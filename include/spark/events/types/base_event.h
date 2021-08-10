
#ifndef SPARK_BASE_EVENT_H
#define SPARK_BASE_EVENT_H

#include "spark/core/utility.h"

namespace Spark {
    namespace Events {

        class IEvent {
            public:
                virtual ~IEvent() = default;
                NODISCARD virtual EventTypeID GetID() const = 0;
        };

    }
}

#endif // SPARK_BASE_EVENT_H
