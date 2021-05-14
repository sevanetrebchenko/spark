
#include <GLFW/glfw3.h>
#include <spark/events/types/key_events.h> // KeyPressEvent, KeyReleaseEvent, Event

namespace Spark::Events {
    // Declaration.
    static std::string KeyButtonToString(int keyCode);

    KeyPressEvent::KeyPressEvent(int keyCode) : Event(EventType::KeyPressed, EventCategory::EventCategoryInput), _keyCode(keyCode) {
        // Nothing to do here.
    }

    std::string KeyPressEvent::ToString() const {
        static std::stringstream format;
        static std::string formattedString;

        try {
            format << KeyButtonToString(_keyCode);
        }
        catch (const std::invalid_argument& exception) {
            format << "Unknown key.";
        }

        formattedString = format.str();
        format.str(std::string()); // Move string data to clear stringstream buffer.
        return std::move(formattedString);
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
        static std::stringstream format;
        static std::string formattedString;

        try {
            format << KeyButtonToString(_keyCode);
        }
        catch (const std::invalid_argument& exception) {
            format << "Unknown key.";
        }

        formattedString = format.str();
        format.str(std::string()); // Move string data to clear stringstream buffer.
        return std::move(formattedString);
    }

    // Utility functions
    std::string KeyButtonToString(int keyCode) {
        static std::stringstream format;
        static std::string formattedString;

        switch (keyCode) {
            case GLFW_KEY_SPACE:
                format << "GLFW_KEY_SPACE (code: " << keyCode << ").";
                break;
            case GLFW_KEY_APOSTROPHE:
                format << "GLFW_KEY_APOSTROPHE (code: " << keyCode << ").";
                break;
            case GLFW_KEY_COMMA:
                format << "GLFW_KEY_COMMA (code: " << keyCode << ").";
                break;
            case GLFW_KEY_MINUS:
                format << "GLFW_KEY_MINUS (code: " << keyCode << ").";
                break;
            case GLFW_KEY_PERIOD:
                format << "GLFW_KEY_PERIOD (code: " << keyCode << ").";
                break;
            case GLFW_KEY_SLASH:
                format << "GLFW_KEY_SLASH (code: " << keyCode << ").";
                break;
            case GLFW_KEY_0:
                format << "GLFW_KEY_0 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_1:
                format << "GLFW_KEY_1 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_2:
                format << "GLFW_KEY_2 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_3:
                format << "GLFW_KEY_3 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_4:
                format << "GLFW_KEY_4 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_5:
                format << "GLFW_KEY_5 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_6:
                format << "GLFW_KEY_6 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_7:
                format << "GLFW_KEY_7 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_8:
                format << "GLFW_KEY_8 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_9:
                format << "GLFW_KEY_9 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_SEMICOLON:
                format << "GLFW_KEY_SEMICOLON (code: " << keyCode << ").";
                break;
            case GLFW_KEY_EQUAL:
                format << "GLFW_KEY_EQUAL (code: " << keyCode << ").";
                break;
            case GLFW_KEY_A:
                format << "GLFW_KEY_A (code: " << keyCode << ").";
                break;
            case GLFW_KEY_B:
                format << "GLFW_KEY_B (code: " << keyCode << ").";
                break;
            case GLFW_KEY_C:
                format << "GLFW_KEY_C (code: " << keyCode << ").";
                break;
            case GLFW_KEY_D:
                format << "GLFW_KEY_D (code: " << keyCode << ").";
                break;
            case GLFW_KEY_E:
                format << "GLFW_KEY_E (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F:
                format << "GLFW_KEY_F (code: " << keyCode << ").";
                break;
            case GLFW_KEY_G:
                format << "GLFW_KEY_G (code: " << keyCode << ").";
                break;
            case GLFW_KEY_H:
                format << "GLFW_KEY_H (code: " << keyCode << ").";
                break;
            case GLFW_KEY_I:
                format << "GLFW_KEY_I (code: " << keyCode << ").";
                break;
            case GLFW_KEY_J:
                format << "GLFW_KEY_J (code: " << keyCode << ").";
                break;
            case GLFW_KEY_K:
                format << "GLFW_KEY_K (code: " << keyCode << ").";
                break;
            case GLFW_KEY_L:
                format << "GLFW_KEY_L (code: " << keyCode << ").";
                break;
            case GLFW_KEY_M:
                format << "GLFW_KEY_M (code: " << keyCode << ").";
                break;
            case GLFW_KEY_N:
                format << "GLFW_KEY_N (code: " << keyCode << ").";
                break;
            case GLFW_KEY_O:
                format << "GLFW_KEY_O (code: " << keyCode << ").";
                break;
            case GLFW_KEY_P:
                format << "GLFW_KEY_P (code: " << keyCode << ").";
                break;
            case GLFW_KEY_Q:
                format << "GLFW_KEY_Q (code: " << keyCode << ").";
                break;
            case GLFW_KEY_R:
                format << "GLFW_KEY_R (code: " << keyCode << ").";
                break;
            case GLFW_KEY_S:
                format << "GLFW_KEY_S (code: " << keyCode << ").";
                break;
            case GLFW_KEY_T:
                format << "GLFW_KEY_T (code: " << keyCode << ").";
                break;
            case GLFW_KEY_U:
                format << "GLFW_KEY_U (code: " << keyCode << ").";
                break;
            case GLFW_KEY_V:
                format << "GLFW_KEY_V (code: " << keyCode << ").";
                break;
            case GLFW_KEY_W:
                format << "GLFW_KEY_W (code: " << keyCode << ").";
                break;
            case GLFW_KEY_X:
                format << "GLFW_KEY_X (code: " << keyCode << ").";
                break;
            case GLFW_KEY_Y:
                format << "GLFW_KEY_Y (code: " << keyCode << ").";
                break;
            case GLFW_KEY_Z:
                format << "GLFW_KEY_Z (code: " << keyCode << ").";
                break;
            case GLFW_KEY_LEFT_BRACKET:
                format << "GLFW_KEY_LEFT_BRACKET (code: " << keyCode << ").";
                break;
            case GLFW_KEY_BACKSLASH:
                format << "GLFW_KEY_BACKSLASH (code: " << keyCode << ").";
                break;
            case GLFW_KEY_RIGHT_BRACKET:
                format << "GLFW_KEY_RIGHT_BRACKET (code: " << keyCode << ").";
                break;
            case GLFW_KEY_GRAVE_ACCENT:
                format << "GLFW_KEY_GRAVE_ACCENT (code: " << keyCode << ").";
                break;
            case GLFW_KEY_ESCAPE:
                format << "GLFW_KEY_ESCAPE (code: " << keyCode << ").";
                break;
            case GLFW_KEY_ENTER:
                format << "GLFW_KEY_ENTER (code: " << keyCode << ").";
                break;
            case GLFW_KEY_TAB:
                format << "GLFW_KEY_TAB (code: " << keyCode << ").";
                break;
            case GLFW_KEY_BACKSPACE:
                format << "GLFW_KEY_BACKSPACE (code: " << keyCode << ").";
                break;
            case GLFW_KEY_INSERT:
                format << "GLFW_KEY_INSERT (code: " << keyCode << ").";
                break;
            case GLFW_KEY_DELETE:
                format << "GLFW_KEY_DELETE (code: " << keyCode << ").";
                break;
            case GLFW_KEY_RIGHT:
                format << "GLFW_KEY_RIGHT (code: " << keyCode << ").";
                break;
            case GLFW_KEY_LEFT:
                format << "GLFW_KEY_LEFT (code: " << keyCode << ").";
                break;
            case GLFW_KEY_DOWN:
                format << "GLFW_KEY_DOWN (code: " << keyCode << ").";
                break;
            case GLFW_KEY_UP:
                format << "GLFW_KEY_UP (code: " << keyCode << ").";
                break;
            case GLFW_KEY_PAGE_UP:
                format << "GLFW_KEY_PAGE_UP (code: " << keyCode << ").";
                break;
            case GLFW_KEY_PAGE_DOWN:
                format << "GLFW_KEY_PAGE_DOWN (code: " << keyCode << ").";
                break;
            case GLFW_KEY_HOME:
                format << "GLFW_KEY_HOME (code: " << keyCode << ").";
                break;
            case GLFW_KEY_END:
                format << "GLFW_KEY_END (code: " << keyCode << ").";
                break;
            case GLFW_KEY_CAPS_LOCK:
                format << "GLFW_KEY_CAPS_LOCK (code: " << keyCode << ").";
                break;
            case GLFW_KEY_SCROLL_LOCK:
                format << "GLFW_KEY_SCROLL_LOCK (code: " << keyCode << ").";
                break;
            case GLFW_KEY_NUM_LOCK:
                format << "GLFW_KEY_NUM_LOCK (code: " << keyCode << ").";
                break;
            case GLFW_KEY_PRINT_SCREEN:
                format << "GLFW_KEY_PRINT_SCREEN (code: " << keyCode << ").";
                break;
            case GLFW_KEY_PAUSE:
                format << "GLFW_KEY_PAUSE (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F1:
                format << "GLFW_KEY_F1 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F2:
                format << "GLFW_KEY_F2 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F3:
                format << "GLFW_KEY_F3 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F4:
                format << "GLFW_KEY_F4 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F5:
                format << "GLFW_KEY_F5 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F6:
                format << "GLFW_KEY_F6 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F7:
                format << "GLFW_KEY_F7 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F8:
                format << "GLFW_KEY_F8 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F9:
                format << "GLFW_KEY_F9 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F10:
                format << "GLFW_KEY_F10 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F11:
                format << "GLFW_KEY_F11 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F12:
                format << "GLFW_KEY_F12 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F13:
                format << "GLFW_KEY_F13 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F14:
                format << "GLFW_KEY_F14 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F15:
                format << "GLFW_KEY_F15 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F16:
                format << "GLFW_KEY_F16 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F17:
                format << "GLFW_KEY_F17 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F18:
                format << "GLFW_KEY_F18 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F19:
                format << "GLFW_KEY_F19 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F20:
                format << "GLFW_KEY_F20 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F21:
                format << "GLFW_KEY_F21 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F22:
                format << "GLFW_KEY_F22 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F23:
                format << "GLFW_KEY_F23 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F24:
                format << "GLFW_KEY_F24 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_F25:
                format << "GLFW_KEY_F25 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_KP_0:
                format << "GLFW_KEY_KP_0 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_KP_1:
                format << "GLFW_KEY_KP_1 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_KP_2:
                format << "GLFW_KEY_KP_2 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_KP_3:
                format << "GLFW_KEY_KP_3 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_KP_4:
                format << "GLFW_KEY_KP_4 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_KP_5:
                format << "GLFW_KEY_KP_5 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_KP_6:
                format << "GLFW_KEY_KP_6 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_KP_7:
                format << "GLFW_KEY_KP_7 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_KP_8:
                format << "GLFW_KEY_KP_8 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_KP_9:
                format << "GLFW_KEY_KP_9 (code: " << keyCode << ").";
                break;
            case GLFW_KEY_KP_DECIMAL:
                format << "GLFW_KEY_KP_DECIMAL (code: " << keyCode << ").";
                break;
            case GLFW_KEY_KP_DIVIDE:
                format << "GLFW_KEY_KP_DIVIDE (code: " << keyCode << ").";
                break;
            case GLFW_KEY_KP_MULTIPLY:
                format << "GLFW_KEY_KP_MULTIPLY (code: " << keyCode << ").";
                break;
            case GLFW_KEY_KP_SUBTRACT:
                format << "GLFW_KEY_KP_SUBTRACT (code: " << keyCode << ").";
                break;
            case GLFW_KEY_KP_ADD:
                format << "GLFW_KEY_KP_ADD (code: " << keyCode << ").";
                break;
            case GLFW_KEY_KP_ENTER:
                format << "GLFW_KEY_KP_ENTER (code: " << keyCode << ").";
                break;
            case GLFW_KEY_KP_EQUAL:
                format << "GLFW_KEY_KP_EQUAL (code: " << keyCode << ").";
                break;
            case GLFW_KEY_LEFT_SHIFT:
                format << "GLFW_KEY_LEFT_SHIFT (code: " << keyCode << ").";
                break;
            case GLFW_KEY_LEFT_CONTROL:
                format << "GLFW_KEY_LEFT_CONTROL (code: " << keyCode << ").";
                break;
            case GLFW_KEY_LEFT_ALT:
                format << "GLFW_KEY_LEFT_ALT (code: " << keyCode << ").";
                break;
            case GLFW_KEY_LEFT_SUPER:
                format << "GLFW_KEY_LEFT_SUPER (code: " << keyCode << ").";
                break;
            case GLFW_KEY_RIGHT_SHIFT:
                format << "GLFW_KEY_RIGHT_SHIFT (code: " << keyCode << ").";
                break;
            case GLFW_KEY_RIGHT_CONTROL:
                format << "GLFW_KEY_RIGHT_CONTROL (code: " << keyCode << ").";
                break;
            case GLFW_KEY_RIGHT_ALT:
                format << "GLFW_KEY_RIGHT_ALT (code: " << keyCode << ").";
                break;
            case GLFW_KEY_RIGHT_SUPER:
                format << "GLFW_KEY_RIGHT_SUPER (code: " << keyCode << ").";
                break;
            case GLFW_KEY_MENU:
                format << "GLFW_KEY_MENU (code: " << keyCode << ").";
                break;
            default:
                throw std::invalid_argument("");
                break;
        }

        formattedString = format.str();
        format.str(std::string()); // Clear stringstream buffer.
        return std::move(formattedString);
    }
}
