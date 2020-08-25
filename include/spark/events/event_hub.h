
#ifndef SPARK_EVENT_HUB_H
#define SPARK_EVENT_HUB_H

#include <spark/events/event_hub_private_interface.h> // IEventHubPrivate
#include <spark/events/event_listener_interface.h>    // IEventListener

namespace Spark {
    namespace Events {

            class EventHub : public IEventHubPrivate {
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
}

#endif // SPARK_EVENT_HUB_H
