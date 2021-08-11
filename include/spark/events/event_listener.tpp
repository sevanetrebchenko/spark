
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
        // Distribute all remaining events.
        while (!eventQueue_.empty()) {
            const EventHandle& data = eventQueue_.front();
            DispatchEvent<EventTypes...>(data);
            eventQueue_.pop();
        }

        Singleton<EventHub>::GetInstance()->DetachEventListener(this);
    }

    template <class CRTP, class... EventTypes>
    void EventListener<CRTP, EventTypes...>::OnEventReceived(const EventHandle& event) {
        const IEvent& data = *event;
        if (ManagesEventType<EventTypes...>(data.GetID())) {
            eventQueue_.emplace(event); // Intentional copy.
        }
    }

    template <class CRTP, class... EventTypes>
    void EventListener<CRTP, EventTypes...>::OnUpdate(float) {
        while (!eventQueue_.empty()) {
            const EventHandle& data = eventQueue_.front();
            DispatchEvent<EventTypes...>(data);
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
    void EventListener<CRTP, EventTypes...>::DispatchEvent(const EventHandle& event) {
        if (event->GetID() == EventType1::ID) {
            // Found correct event type, cast it to appropriate type to pass into OnEvent functions.
            std::shared_ptr<const EventType1> derived = std::dynamic_pointer_cast<const EventType1>(event);
            SP_ASSERT(derived, "Sanity check: failed to get registered derived event type from base event type.");
            static_cast<CRTP*>(this)->OnEvent(&(*derived)); // Address of data.
        }
        else {
            DispatchEvent<EventType2, AdditionalEventTypes...>(event);
        }
    }

    template <class CRTP, class... EventTypes>
    template <class EventType>
    void EventListener<CRTP, EventTypes...>::DispatchEvent(const EventHandle& event) {
        SP_ASSERT(event->GetID() == EventType::ID, "Sanity check - failed to get correct type of event.");
        std::shared_ptr<const EventType> derived = std::dynamic_pointer_cast<const EventType>(event);
        SP_ASSERT(derived, "Sanity check: failed to get registered derived event type from base event type.");
        static_cast<CRTP*>(this)->OnEvent(&(*derived)); // Address of data.
    }

}

#endif // SPARK_EVENT_LISTENER_TPP
