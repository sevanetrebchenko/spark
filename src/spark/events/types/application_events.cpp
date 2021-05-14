
#include <spark/events/types/application_events.h> // WindowResizeEvent, WindowCloseEvent, Event

namespace Spark::Events {
    //------------------------------------------------------------------------------------------------------------------
    // WINDOW RESIZE EVENT
    //------------------------------------------------------------------------------------------------------------------
    WindowResizeEvent::WindowResizeEvent(int width, int height) : Event(EventType::WindowResized, EventCategory::EventCategoryApplication), _width(width), _height(height) {
        // Nothing to do here.
    }

    std::string WindowResizeEvent::ToString() const {
        static std::stringstream format;
        static std::string formattedString;
        format << "Updated window dimensions: (" << _width << ", " << _height << ").";
        formattedString = format.str();
        format.str(std::string()); // Move string data to clear stringstream buffer.
        return std::move(formattedString);
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
        return "N/A";
    }


    //------------------------------------------------------------------------------------------------------------------
    // WINDOW MINIMIZED EVENT
    //------------------------------------------------------------------------------------------------------------------
    WindowMinimizedEvent::WindowMinimizedEvent(bool minimized) : Event(EventType::WindowMinimized, EventCategory::EventCategoryApplication), _minimized(minimized) {
        // Nothing to do here.
    }

    std::string WindowMinimizedEvent::ToString() const {
        if (_minimized) {
            return "Window minimized.";
        }
        else {
            return "Window restored.";
        }
    }

    bool WindowMinimizedEvent::GetMinimized() const {
        return _minimized;
    }
}
