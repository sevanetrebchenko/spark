
#include <spark/events/types/event.h> // Event

namespace Spark::Events {

    IEvent::IEvent(EventType eventType, EventCategory eventCategory) : _eventType(eventType), _eventCategory(eventCategory) {
        // Nothing to do here.
    }

    const EventType& IEvent::GetEventType() const {
        return _eventType;
    }

    const EventCategory& IEvent::GetEventCategory() const {
        return _eventCategory;
    }

    bool IEvent::IsInEventCategory(EventCategory eventCategory) const {
        return true;//_eventCategory;// & eventCategory;
    }

    std::string IEvent::ConvertEventTypeToString(const EventType& eventType) {
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
            default:
                SP_ASSERT(false, ""); // TODO
                return "";
        }
    }

}



