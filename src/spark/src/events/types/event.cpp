
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

    std::string Event::ConvertEventTypeToString(const EventType& eventType) {
        switch (eventType) {
            case EventType::None:
                return "None";
            case EventType::WindowCloseRequested:
                return "WindowCloseRequested";
            case EventType::WindowResized:
                return "WindowResized";
            case EventType::WindowMinimized:
                return "WindowMinimized";
            case EventType::KeyPressed:
                return "KeyPressed";
            case EventType::KeyReleased:
                return "KeyReleased";
            case EventType::MouseButtonPressed:
                return "MouseButtonPressed";
            case EventType::MouseButtonReleased:
                return "MouseButtonReleased";
            case EventType::MouseScrolled:
                return "MouseScrolled";
            case EventType::MouseMoved:
                return "MouseMoved";
            case EventType::EntityComponentAdd:
                return "EntityComponentAdd";
            case EventType::EntityComponentRemove:
                return "EntityComponentRemove";
        }
    }

}



