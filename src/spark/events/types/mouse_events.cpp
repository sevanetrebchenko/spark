
#include <GLFW/glfw3.h>
#include <spark/events/types/mouse_events.h> // MouseButtonPressedEvent, MouseButtonReleasedEvent, MouseScrolledEvent, MouseMovedEvent, Event

namespace Spark::Events {

    // Declaration.
    static std::string MouseButtonToString(int mouseButton);

    MouseButtonPressedEvent::MouseButtonPressedEvent(int mouseButtonCode) : IEvent(EventType::MouseButtonPressed, EventCategory::EventCategoryInput), _mouseButtonCode(mouseButtonCode) {
        // Nothing to do here.
    }

    std::string MouseButtonPressedEvent::ToString() const {
        static std::stringstream format;
        static std::string formattedString;

        try {
            format << MouseButtonToString(_mouseButtonCode);
        }
        catch (const std::invalid_argument& exception) {
            format << "Unknown mouse button.";
        }

        formattedString = format.str();
        format.str(std::string()); // Move string data to clear stringstream buffer.
        return std::move(formattedString);
    }

    int MouseButtonPressedEvent::GetMouseButtonCode() const {
        return _mouseButtonCode;
    }

    MouseButtonReleasedEvent::MouseButtonReleasedEvent(int mouseButtonCode) : IEvent(EventType::MouseButtonReleased, EventCategory::EventCategoryInput), _mouseButtonCode(mouseButtonCode) {
        // Nothing to do here.
    }

    std::string MouseButtonReleasedEvent::ToString() const {
        static std::stringstream format;
        static std::string formattedString;

        try {
            format << MouseButtonToString(_mouseButtonCode);
        }
        catch (const std::invalid_argument& exception) {
            format << "Unknown mouse button.";
        }

        formattedString = format.str();
        format.str(std::string()); // Move string data to clear stringstream buffer.
        return std::move(formattedString);
    }

    int MouseButtonReleasedEvent::GetMouseButtonCode() const {
        return _mouseButtonCode;
    }

    MouseScrolledEvent::MouseScrolledEvent(double scrollX, double scrollY) : IEvent(EventType::MouseScrolled, EventCategory::EventCategoryInput), _scrollX(scrollX), _scrollY(scrollY) {
        // Nothing to do here.
    }

    std::string MouseScrolledEvent::ToString() const {
        static std::stringstream format;
        static std::string formattedString;
        format << "Horizontal scroll: " << _scrollX << ". Vertical scroll: " << _scrollY << ".";
        formattedString = format.str();
        format.str(std::string()); // Move string data to clear stringstream buffer.
        return std::move(formattedString);
    }

    double MouseScrolledEvent::GetScrollX() const {
        return _scrollX;
    }

    double MouseScrolledEvent::GetScrollY() const {
        return _scrollY;
    }

    MouseMovedEvent::MouseMovedEvent(int mouseX, int mouseY) : IEvent(EventType::MouseMoved, EventCategory::EventCategoryInput), _mouseX(mouseX), _mouseY(mouseY) {
        // Nothing to do here.
    }

    std::string MouseMovedEvent::ToString() const {
        static std::stringstream format;
        static std::string formattedString;
        format << "New mouse position: (" << _mouseX << ", " << _mouseY << ").";
        formattedString = format.str();
        format.str(std::string()); // Move string data to clear stringstream buffer.
        return std::move(formattedString);
    }

    int MouseMovedEvent::GetMouseX() const {
        return _mouseX;
    }

    int MouseMovedEvent::GetMouseY() const {
        return _mouseY;
    }

    // Utility functions
    std::string MouseButtonToString(int mouseButton) {
        static std::stringstream format;
        static std::string formattedString;

        switch (mouseButton) {
            // Left mouse button.
            case GLFW_MOUSE_BUTTON_1:
                format << "GLFW_MOUSE_BUTTON_LEFT (code: " << mouseButton << ").";
                break;
            // Right mouse button.
            case GLFW_MOUSE_BUTTON_2:
                format << "GLFW_MOUSE_BUTTON_RIGHT (code: " << mouseButton << ").";
                break;
            // Middle mouse button
            case GLFW_MOUSE_BUTTON_3:
                format << "GLFW_MOUSE_BUTTON_MIDDLE (code: " << mouseButton << ").";
                break;
            case GLFW_MOUSE_BUTTON_4:
                format << "GLFW_MOUSE_BUTTON_4 (code: " << mouseButton << ").";
                break;
            case GLFW_MOUSE_BUTTON_5:
                format << "GLFW_MOUSE_BUTTON_5 (code: " << mouseButton << ").";
                break;
            case GLFW_MOUSE_BUTTON_6:
                format << "GLFW_MOUSE_BUTTON_6 (code: " << mouseButton << ").";
                break;
            case GLFW_MOUSE_BUTTON_7:
                format << "GLFW_MOUSE_BUTTON_7 (code: " << mouseButton << ").";
                break;
            case GLFW_MOUSE_BUTTON_8:
                format << "GLFW_MOUSE_BUTTON_8 (code: " << mouseButton << ").";
                break;
            // Should never happen.
            default:
                throw std::invalid_argument("");
        }

        formattedString = format.str();
        format.str(std::string()); // Clear stringstream buffer.
        return std::move(formattedString);
    }

}
