
#include <spark/events/types/key_events.h> // KeyPressEvent, KeyReleaseEvent, Event

namespace Spark::Events {

    KeyPressEvent::KeyPressEvent(int keyCode) : Event(EventType::KeyPressed, EventCategory::EventCategoryInput), _keyCode(keyCode) {
        // Nothing to do here.
    }

    std::string KeyPressEvent::ToString() const {
        return "keypressed";
    }

    int KeyPressEvent::GetKeyCode() const {
        return _keyCode;
    }


    KeyReleaseEvent::KeyReleaseEvent(int keyCode) : Event(EventType::KeyReleased, EventCategory::EventCategoryInput), _keyCode(keyCode) {
        // Nothing to do here.
    }

    int KeyReleaseEvent::GetKeyCode() const {
        return _keyCode;
    }

    std::string KeyReleaseEvent::ToString() const {
        return "keyrelease";
    }
}
