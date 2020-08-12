#include <events/event.h>

namespace Spark::Events {
    Event::Event(EventType eventType, EventCategory eventCategory) : _eventType(eventType), _eventCategory(eventCategory) {
        // Nothing to do here.
    }

    EventType Event::GetEventType() {
        return _eventType;
    }
}



