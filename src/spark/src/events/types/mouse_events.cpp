
#include <spark/events/types/mouse_events.h> // MouseButtonPressedEvent, MouseButtonReleasedEvent, MouseScrolledEvent, MouseMovedEvent, Event

namespace Spark::Events {

    MouseButtonPressedEvent::MouseButtonPressedEvent(int mouseButtonCode) : Event(EventType::MouseButtonPressed, EventCategory::EventCategoryInput), _mouseButtonCode(mouseButtonCode) {
        // Nothing to do here.
    }

    std::string MouseButtonPressedEvent::ToString() const {
        return "mousepressed";
    }

    int MouseButtonPressedEvent::GetMouseButtonCode() const {
        return _mouseButtonCode;
    }

    MouseButtonReleasedEvent::MouseButtonReleasedEvent(int mouseButtonCode) : Event(EventType::MouseButtonReleased, EventCategory::EventCategoryInput), _mouseButtonCode(mouseButtonCode) {
        // Nothing to do here.
    }

    std::string MouseButtonReleasedEvent::ToString() const {
        return "mousereleased";
    }

    int MouseButtonReleasedEvent::GetMouseButtonCode() const {
        return _mouseButtonCode;
    }

    MouseScrolledEvent::MouseScrolledEvent(double scrollX, double scrollY) : Event(EventType::MouseScrolled, EventCategory::EventCategoryInput), _scrollX(scrollX), _scrollY(scrollY) {
        // Nothing to do here.
    }

    std::string MouseScrolledEvent::ToString() const {
        return "mousescroll";
    }

    double MouseScrolledEvent::GetScrollX() const {
        return _scrollX;
    }

    double MouseScrolledEvent::GetScrollY() const {
        return _scrollY;
    }

    MouseMovedEvent::MouseMovedEvent(int mouseX, int mouseY) : Event(EventType::MouseMoved, EventCategory::EventCategoryInput), _mouseX(mouseX), _mouseY(mouseY) {
        // Nothing to do here.
    }

    std::string MouseMovedEvent::ToString() const {
        return "mousemoved";
    }

    int MouseMovedEvent::GetMouseX() const {
        return _mouseX;
    }

    int MouseMovedEvent::GetMouseY() const {
        return _mouseY;
    }


}
