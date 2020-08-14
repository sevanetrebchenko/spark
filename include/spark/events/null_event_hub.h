
#ifndef SPARK_NULL_EVENT_HUB_H
#define SPARK_NULL_EVENT_HUB_H

#include <events/event_hub_interface.h>

namespace Spark {
    namespace Events {

        class NullEventHub : public IEventHub {
            public:
                void AttachEventListener(IEventListener* eventListener) override;
                void DetachEventListener(IEventListener* eventListener) override;
                void Dispatch(Event* event) override;
        };

    }
}

#endif //SPARK_NULL_EVENT_HUB_H
