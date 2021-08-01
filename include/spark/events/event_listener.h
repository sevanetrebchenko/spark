
#ifndef SPARK_EVENT_LISTENER_H
#define SPARK_EVENT_LISTENER_H

#include <spark/core/rename.h>
#include <spark/events/types/event.h>        // Event
#include <spark/events/event_listener_interface.h> // IEventListener

namespace Spark {
    namespace Events {

        // Template parameters for EventListener registration must derive from base Event class.
        template <class ...EventTypes>
        class EventListener : public IEventListener {
            public:
                explicit EventListener(const std::string& name, const std::function<void(std::queue<std::shared_ptr<IEvent*>>&)>& eventProcessingFunction);
                ~EventListener();

                NODISCARD bool OnEventReceived(std::shared_ptr<IEvent*> eventPointer) override;
                NODISCARD const char* GetName() override;

            private:
                void OnUpdate() override;
                NODISCARD const std::vector<std::string>& GetEventTypesAsStrings() override;

                NODISCARD const char* GetName() const;

                template <class EventType>
                void AppendEventTypeAsString(std::vector<std::string>& eventTypes);

                template <class EventType1, class EventType2, class ...AdditionalEventTypes>
                NODISCARD bool ManagesEventType(const EventType& eventType);

                template<class EventType>
                NODISCARD bool ManagesEventType(const EventType& eventType);

                std::string _name;
                std::function<void(std::queue<std::shared_ptr<IEvent*>>&)> _processingFunction;
                std::queue<std::shared_ptr<IEvent*>> _eventQueue;
        };

    }
}

#include <spark/events/event_listener.tpp> // Template function definitions.

#endif // SPARK_EVENT_LISTENER_H
