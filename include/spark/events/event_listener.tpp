
#ifndef SPARK_EVENT_LISTENER_TPP
#define SPARK_EVENT_LISTENER_TPP

namespace Spark::Events {

    template <class CRTP, class... EventTypes>
    EventListener<CRTP, EventTypes...>::EventListener() {
        static_assert((std::is_base_of_v<IEvent, EventTypes> && ...), "Invalid template parameter passed to EventListener constructor - event types must derive from the IEvent base class.");
        Singleton<EventHub>::GetInstance()->AttachEventListener(this);
    }

    template <class CRTP, class... EventTypes>
    EventListener<CRTP, EventTypes...>::~EventListener() {
        // Clear all remaining events.
        while (!eventQueue_.empty()) {
            eventQueue_.pop();
        }

        Singleton<EventHub>::GetInstance()->DetachEventListener(this);
    }

    template <class CRTP, class... EventTypes>
    void EventListener<CRTP, EventTypes...>::OnEventReceived(std::shared_ptr<const IEvent*> eventPtr) {
        const IEvent* event = *eventPtr; // Guaranteed to exist.
        SP_ASSERT(event, "Provided event pointer to OnEventReceived is null.");

        if (ManagesEventType<EventTypes...>(event->GetID())) {
            eventQueue_.emplace(eventPtr);
        }
    }

    template <class CRTP, class... EventTypes>
    void EventListener<CRTP, EventTypes...>::OnUpdate(float) {
        while (!eventQueue_.empty()) {
            const std::shared_ptr<const IEvent*>& data = eventQueue_.front(); // Create reference to not increase ref count.
            auto event = GetEventOfType<EventTypes...>(*data);
            SP_ASSERT(event, "Provided event pointer to OnUpdate is null.");
            static_cast<CRTP*>(this)->OnEvent(event);
            eventQueue_.pop();
        }
    }

    template <class CRTP, class... EventTypes>
    template <class EventType1, class EventType2, class ...AdditionalEventTypes>
    bool EventListener<CRTP, EventTypes...>::ManagesEventType(const EventTypeID& eventType) {
        if (eventType == EventType1::ID) {
            return true;
        }
        else {
            return ManagesEventType<EventType2, AdditionalEventTypes...>(eventType);
        }
    }

    template <class CRTP, class... EventTypes>
    template <class EventType>
    bool EventListener<CRTP, EventTypes...>::ManagesEventType(const EventTypeID& eventType) {
        return eventType == EventType::ID;
    }

    template <class CRTP, class... EventTypes>
    template <class EventType1, class EventType2, class ...AdditionalEventTypes>
    const EventType1* EventListener<CRTP, EventTypes...>::GetEventOfType(const IEvent* baseEvent) {
        if (const EventType1* event = dynamic_cast<const EventType1*>(baseEvent)) {
            return event;
        }
        else {
            GetEventOfType<EventType2, AdditionalEventTypes...>(baseEvent);
        }
    }

    template <class CRTP, class... EventTypes>
    template <class EventType>
    const EventType* EventListener<CRTP, EventTypes...>::GetEventOfType(const IEvent* baseEvent) {
        const EventType* event = dynamic_cast<const EventType*>(baseEvent);
        SP_ASSERT(event, "Failed to acquire correct event type in function GetEventOfType."); // Should never happen.
        return event;
    }

}

#endif // SPARK_EVENT_LISTENER_TPP
