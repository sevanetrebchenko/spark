
#include <spark/events/types/application_events.h> // WindowResizeEvent, WindowCloseEvent, Event

namespace Spark::Events {
    //------------------------------------------------------------------------------------------------------------------
    // WINDOW RESIZE EVENT
    //------------------------------------------------------------------------------------------------------------------
    WindowResizeEvent::WindowResizeEvent(int width, int height) : Event(EventType::WindowResized, EventCategory::EventCategoryApplication), _width(width), _height(height) {
        // Nothing to do here.
    }

    std::string WindowResizeEvent::ToString() const {
        static std::stringstream stream;
        stream << "[WindowResizeEvent] - Updated window dimensions: (" << _width << ", " << _height << ").";
        return std::move(stream.str()); // Move string data to clear stringstream buffer.
    }

    int WindowResizeEvent::GetWidth() const {
        return _width;
    }

    int WindowResizeEvent::GetHeight() const {
        return _height;
    }


    //------------------------------------------------------------------------------------------------------------------
    // WINDOW CLOSE EVENT
    //------------------------------------------------------------------------------------------------------------------
    WindowCloseEvent::WindowCloseEvent() : Event(EventType::WindowCloseRequested, EventCategory::EventCategoryApplication) {
        // Nothing to do here.
    }

    std::string WindowCloseEvent::ToString() const {
        static std::stringstream stream;
        stream << "[WindowCloseRequested]";
        return std::move(stream.str()); // Move string data to clear stringstream buffer.
    }


    //------------------------------------------------------------------------------------------------------------------
    // WINDOW MINIMIZED EVENT
    //------------------------------------------------------------------------------------------------------------------
    WindowMinimizedEvent::WindowMinimizedEvent(bool minimized) : Event(EventType::WindowMinimized, EventCategory::EventCategoryApplication), _minimized(minimized) {
        // Nothing to do here.
    }

    std::string WindowMinimizedEvent::ToString() const {
        return "[WindowMinimized]";
    }

    bool WindowMinimizedEvent::GetMinimized() const {
        return _minimized;
    }
}
