
#include <events/application_events.h>

namespace Spark::Events {

    WindowResizeEvent::WindowResizeEvent(unsigned int width, unsigned int height) : Event(EventType::WindowResized, EventCategory::EventCategoryApplication), _width(width), _height(height) {
        // Nothing to do here.
    }

    unsigned WindowResizeEvent::GetWidth() const {
        return _width;
    }

    unsigned WindowResizeEvent::GetHeight() const {
        return _height;
    }

    WindowCloseEvent::WindowCloseEvent() : Event(EventType::WindowCloseRequested, EventCategory::EventCategoryApplication) {

    }
}
