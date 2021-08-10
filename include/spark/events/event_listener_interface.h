
#ifndef SPARK_EVENT_LISTENER_INTERFACE_H
#define SPARK_EVENT_LISTENER_INTERFACE_H

#include "spark/core/utility.h"
#include "spark/events/types/base_event.h"

namespace Spark {
    namespace Events {

        class IEventListener {
            public:
                virtual void OnEventReceived(std::shared_ptr<const IEvent*> event) = 0;
                virtual void OnUpdate(float dt) = 0;
        };

    }
}

#endif //SPARK_EVENT_LISTENER_INTERFACE_H
