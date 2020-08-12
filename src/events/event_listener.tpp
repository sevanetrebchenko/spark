
#include <events/event_listener.h>
#include <events/event.h>
#include <spark_pch.h>

namespace Spark::Events {
    template <class ...EventTypes>
    class EventListener<EventTypes...>::EventListenerData {
        public:
            EventListenerData();
            ~EventListenerData();

            void OnEventReceived(std::shared_ptr<Event*> eventPointer);

        private:
            /**
             * Check whether this EventListener is registered to receive the given type of event.
             * @param eventType - Type of event.
             * @return True: Events of type 'eventType' are registered with the EventListener.
             *         False: Events of type 'eventType' are not registered with the EventListener.
             */
            template <unsigned INDEX, class CurrentEventType, class ...AdditionalEventArgs>
            bool ManagesEventType(const EventType& eventType);

            template <unsigned INDEX>
            bool ManagesEventType(const EventType& eventType);

            std::queue<Event*> _eventQueue;
    };

    template<class... EventTypes>
    void EventListener<EventTypes...>::EventListenerData::OnEventReceived(std::shared_ptr<Event*> eventPointer) {
        Event& event = **eventPointer;
        if (ManagesEventType<0, EventTypes...>(event.GetEventType())) {
            std::cout << "found type" << std::endl;
        }
    }

    template<class... EventTypes>
    template<unsigned int INDEX, class CurrentEventType, class... AdditionalEventArgs>
    bool EventListener<EventTypes...>::EventListenerData::ManagesEventType(const EventType &eventType) {
        if (eventType == CurrentEventType::GetStaticEventType()) {
            return true;
        }
        else {
            return ManagesEventType<INDEX + 1, AdditionalEventArgs...>(eventType);
        }
    }

    template<class... EventTypes>
    template<unsigned int INDEX>
    bool EventListener<EventTypes...>::EventListenerData::ManagesEventType(const EventType &eventType) {
        return false;
    }

    template<class... EventTypes>
    EventListener<EventTypes...>::EventListener() {
        // Check all eve
        static_assert((std::is_base_of_v<Event, EventTypes> && ...), "Invalid template parameter passed to EventListener constructor - provided event type does not derive from 'Event' and is therefore not a valid event.");
    }

    template<class... EventTypes>
    void EventListener<EventTypes...>::OnEventReceived(std::shared_ptr<Event*> eventPointer) {
        _data->OnEventReceived(eventPointer);
    }

    template<class... EventTypes>
    EventListener<EventTypes...>::~EventListener() {

    }
}
