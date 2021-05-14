
#ifndef SPARK_EVENT_LISTENER_H
#define SPARK_EVENT_LISTENER_H

#include <spark/core/core.h>
#include <spark/events/types/event.h>        // Event
#include <spark/events/event_listener_interface.h> // IEventListener

namespace Spark {
    namespace Events {

        // Template parameters for EventListener registration must derive from base Event class.
        template <class ...EventTypes>
        class EventListener : public IEventListener {
            public:
                explicit EventListener(const char* name, const std::function<void(std::queue<std::shared_ptr<Event*>>&)>& eventProcessingFunction);
                ~EventListener();

                _NODISCARD_ bool OnEventReceived(std::shared_ptr<Event*> eventPointer) override;
                _NODISCARD_ const char* GetName() override;

            private:
                void OnUpdate() override;
                _NODISCARD_ const std::vector<std::string>& GetEventTypesAsStrings() override;

                class EventListenerData;
                EventListenerData* _data;
        };

    }
}

#include <spark/events/event_listener.tpp> // Template function definitions.

#endif // SPARK_EVENT_LISTENER_H
