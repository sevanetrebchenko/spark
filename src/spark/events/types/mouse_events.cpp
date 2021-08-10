
#include "spark/events/types/mouse_events.h"

namespace Spark::Events {

    // MouseButtonPressedEvent.
    MouseButtonPressedEvent::MouseButtonPressedEvent(int mouseButtonCode) : mouseButtonCode_(mouseButtonCode) {
    }

    int MouseButtonPressedEvent::GetMouseButtonCode() const {
        return mouseButtonCode_;
    }


    // MouseButtonPressedEvent.
    MouseButtonReleasedEvent::MouseButtonReleasedEvent(int mouseButtonCode) : mouseButtonCode_(mouseButtonCode) {
    }

    int MouseButtonReleasedEvent::GetMouseButtonCode() const {
        return mouseButtonCode_;
    }


    // MouseScrolledEvent.
    MouseScrolledEvent::MouseScrolledEvent(double scrollX, double scrollY) : scrollX_(scrollX),
                                                                             scrollY_(scrollY)
                                                                             {
    }

    double MouseScrolledEvent::GetScrollX() const {
        return scrollX_;
    }

    double MouseScrolledEvent::GetScrollY() const {
        return scrollY_;
    }


    // MouseMovedEvent.
    MouseMovedEvent::MouseMovedEvent(int mouseX, int mouseY) : mouseX_(mouseX),
                                                               mouseY_(mouseY)
                                                               {
    }

    int MouseMovedEvent::GetMouseX() const {
        return mouseX_;
    }

    int MouseMovedEvent::GetMouseY() const {
        return mouseY_;
    }

}
