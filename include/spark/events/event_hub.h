
#ifndef SPARK_EVENT_HUB_H
#define SPARK_EVENT_HUB_H

#include "event_listener_interface.h"

namespace Spark {
    namespace Events {

            class EventHub {
                public:
                    static EventHub* GetInstance();
                    void AttachEventListener(EventListenerInterface* eventListener);
                    void DetachEventListener(EventListenerInterface* eventListener);

                    void Dispatch(Event* event);

                private:
                    EventHub();
                    ~EventHub();

                    static EventHub* _instance;

                    class EventHubData;
                    EventHubData* _data;
            };

    }
} // namespace Spark

#endif // SPARK_EVENT_HUB_H
