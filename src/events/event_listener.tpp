
#ifndef SPARK_EVENT_LISTENER_TPP
#define SPARK_EVENT_LISTENER_TPP

#include <spark_pch.h>

namespace Spark::Events {
    //------------------------------------------------------------------------------------------------------------------
    // EVENT LISTENER DATA
    //------------------------------------------------------------------------------------------------------------------
    template <class ...EventTypes>
    class EventListener<EventTypes...>::EventListenerData {
        public:
            EventListenerData() = default;
            ~EventListenerData();

            void OnEventReceived(std::shared_ptr<const Event*> eventPointer);

        private:
            /**
             * Check whether this EventListener is registered to receive the given type of event.
             * @param eventType - Type of event.
             * @return True: Events of type 'eventType' are registered with the EventListener.
             *         False: Events of type 'eventType' are not registered with the EventListener.
             */
            template <unsigned INDEX, class CurrentEventType, class ...AdditionalEventTypeArgs>
            bool ManagesEventType(const EventType& eventType);

            template<unsigned>
            bool ManagesEventType(const EventType& eventType);

            std::queue<const Event*> _eventQueue;
    };

    template<class... EventTypes>
    EventListener<EventTypes...>::EventListenerData::~EventListenerData() {
        while (!_eventQueue.empty()) {
            _eventQueue.pop();
        }
    }

    template<class... EventTypes>
    void EventListener<EventTypes...>::EventListenerData::OnEventReceived(std::shared_ptr<const Event*> eventPointer) {
        const Event* event = *eventPointer;
        if (ManagesEventType<0u, EventTypes...>(event->GetEventType())) {
            std::cout << "found type" << std::endl;
            _eventQueue.push(event);
        }
    }

    template<class... EventTypes>
    template<unsigned INDEX, class CurrentEventType, class... AdditionalEventTypeArgs>
    bool EventListener<EventTypes...>::EventListenerData::ManagesEventType(const EventType &eventType) {
        if (eventType == CurrentEventType::GetStaticEventType()) {
            return true;
        }
        else {
            return ManagesEventType<INDEX + 1, AdditionalEventTypeArgs...>(eventType);
        }
    }

    template<class... EventTypes>
    template <unsigned>
    bool EventListener<EventTypes...>::EventListenerData::ManagesEventType(const EventType &eventType) {
        return false;
    }


    //------------------------------------------------------------------------------------------------------------------
    // EVENT LISTENER
    //------------------------------------------------------------------------------------------------------------------
    template<class... EventTypes>
    EventListener<EventTypes...>::EventListener() : _data(new EventListenerData()) {
        static_assert((std::is_base_of_v<Event, EventTypes> && ...), "Invalid template parameter passed to EventListener constructor - provided event type does not derive from 'Event' and is therefore not a valid event.");
    }

    template<class... EventTypes>
    EventListener<EventTypes...>::~EventListener() {
        delete _data;
    }

    template<class... EventTypes>
    void EventListener<EventTypes...>::OnEventReceived(std::shared_ptr<const Event*> eventPointer) {
        _data->OnEventReceived(eventPointer);
    }
}

#endif // SPARK_EVENT_LISTENER_TPP
