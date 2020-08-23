
#include <spark/events/types/event.h> // Event

namespace Spark::Events {
    Event::Event(EventType eventType, EventCategory eventCategory) : _eventType(eventType), _eventCategory(eventCategory) {
        // Nothing to do here.
    }

    const EventType& Event::GetEventType() const {
        return _eventType;
    }

    const EventCategory& Event::GetEventCategory() const {
        return _eventCategory;
    }

    bool Event::IsInEventCategory(EventCategory eventCategory) const {
        return _eventCategory & eventCategory;
    }

}



