#ifndef SPARK_EVENT_LISTENER_TPP
#define SPARK_EVENT_LISTENER_TPP

namespace Spark::Events {

    template<class... EventTypes>
    EventListener<EventTypes...>::EventListener(const std::string& name, const std::function<void(std::queue<std::shared_ptr<IEvent*>>&)>& eventProcessingFunction) : _name(name),
                                                                                                                                                                     _processingFunction(eventProcessingFunction)
    {
        static_assert((std::is_base_of_v<IEvent, EventTypes> && ...), "Invalid template parameter passed to EventListener constructor - provided event type does not derive from 'Event' and is therefore not a valid event.");
    }

    template<class... EventTypes>
    EventListener<EventTypes...>::~EventListener() {
        while (!_eventQueue.empty()) {
            _eventQueue.pop();
        }
    }

    template<class... EventTypes>
    bool EventListener<EventTypes...>::OnEventReceived(std::shared_ptr<IEvent*> eventPointer) {
        const IEvent* event = *eventPointer;
        if (ManagesEventType<0u, EventTypes...>(event->GetEventType())) {
            _eventQueue.push(eventPointer);
            return true;
        }

        return false;
    }

    template<class... EventTypes>
    void EventListener<EventTypes...>::OnUpdate() {
        _processingFunction(_eventQueue);
    }

    template<class... EventTypes>
    const std::vector<std::string>& EventListener<EventTypes...>::GetEventTypesAsStrings() {
        static bool initialized = false;
        static std::vector<std::string> eventTypes;

        if (!initialized) {
            PARAMETER_PACK_EXPAND(AppendEventTypeAsString, EventTypes, eventTypes);
        }

        return eventTypes;
    }

    template<class... EventTypes>
    const char *EventListener<EventTypes...>::GetName() {
        return _name.c_str();
    }

    template<class... EventTypes>
    template<class EventType1, class EventType2, class ...AdditionalEventTypes>
    bool EventListener<EventTypes...>::ManagesEventType(const EventType &eventType) {
        if (eventType == EventType1::StaticEventType) {
            return true;
        }
        else {
            return ManagesEventType<EventType2, AdditionalEventTypes...>(eventType);
        }
    }

    template<class... EventTypes>
    template <class EventType>
    bool EventListener<EventTypes...>::ManagesEventType(const EventType &eventType) {
        return eventType == EventType::StaticEventType;
    }

    template<class... EventTypes>
    template<class EventType>
    void EventListener<EventTypes...>::AppendEventTypeAsString(std::vector<std::string> &eventTypes) {
        eventTypes.emplace_back(IEvent::ConvertEventTypeToString(EventType::StaticEventType));
    }

    template<class... EventTypes>
    const char *EventListener<EventTypes...>::GetName() const {
        return _name.c_str();
    }

}

#endif // SPARK_EVENT_LISTENER_TPP
