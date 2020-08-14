
#include <events/application_events.h>
#include <spark_pch.h>

namespace Spark::Events {
    //------------------------------------------------------------------------------------------------------------------
    // WINDOW RESIZE EVENT
    //------------------------------------------------------------------------------------------------------------------
    WindowResizeEvent::WindowResizeEvent(unsigned int width, unsigned int height) : Event(EventType::WindowResized, EventCategory::EventCategoryApplication), _width(width), _height(height) {
        // Nothing to do here.
    }

    std::string WindowResizeEvent::ToString() const {
        static std::stringstream stream;
        stream << "[WindowResizeEvent] - Updated window dimensions: (" << _width << ", " << _height << ").";
        return std::move(stream.str()); // Move string data to clear stringstream buffer.
    }

    unsigned WindowResizeEvent::GetWidth() const {
        return _width;
    }

    unsigned WindowResizeEvent::GetHeight() const {
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
}
