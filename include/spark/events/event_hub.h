
#ifndef SPARK_EVENT_HUB_H
#define SPARK_EVENT_HUB_H

#include "event_listener_interface.h"
#include "event_hub_interface.h"

namespace Spark {
    namespace Events {

            class EventHub : public IEventHub {
                public:
                    static EventHub* GetInstance();
                    void AttachEventListener(IEventListener* eventListener) override;
                    void DetachEventListener(IEventListener* eventListener) override;
                    void Dispatch(Event* event) override;
                    void OnUpdate() override;

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
