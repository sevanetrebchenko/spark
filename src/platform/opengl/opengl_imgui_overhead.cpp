
#include <platform/opengl/imgui_overhead.h>
#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_glfw.h>
#include <imgui.h>
#include <platform/opengl/opengl_window.h>

#define GLFW_VERSION_HAS_NEW_CURSORS (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3400)

namespace Spark::Platform::OpenGL {

    class OpenGLImGuiOverhead::ImGuiOverheadData {
        public:
            /**
             * Initialize ImGui for OpenGL given a fully initialized GLFW window.
             * @param window      - Window to use to initialize ImGui.
             *                      Note: Window must be in a fully initialized and valid state in order for
             * @param glslVersion
             */
            ImGuiOverheadData(GLFWwindow* window, const char* glslVersion);
            ~ImGuiOverheadData();

            void StartFrame();
            void RenderFrame();
            void EndFrame();

        private:
            void SetupCallbacks();
            void SetupKeyMap();
            void SetupMouseCursors();

            static void GLFWMouseButtonCallback(GLFWwindow* /* WINDOW UNUSED */, int mouseButton, int buttonAction, int /* MODS UNUSED */);
            static void ButtonIDToString(UtilityBox::Logger::LogMessage& message, int mouseButton);
            static void ButtonActionToString(UtilityBox::Logger::LogMessage& message, int buttonAction);

            static void GLFWMouseScrollCallback(GLFWwindow* /* WINDOW UNUSED */, double xOffset, double yOffset);

            static void GLFWKeyCallback(GLFWwindow* /* WINDOW UNUSED */, int keyCode, int /* SCANCODE UNUSED */, int keyAction, int /* MODS UNUSED */);
            static void KeyStateToString(UtilityBox::Logger::LogMessage& message, int keyCode, int keyAction);
            static void KeyButtonToString(UtilityBox::Logger::LogMessage& message, int keyCode, const char* actionString);

            static const char* GLFWGetClipboardText(void* window);
            static void GLFWSetClipboardText(void* window, const char* text);

            void GLFWNewFrame(UtilityBox::Logger::LogMessage& message);
            void UpdateWindowSize();
            void UpdateMousePosition();
            void UpdateMouseCursor();

            void SetupImGuiOpenGLEnvironment(UtilityBox::Logger::LogMessage& message);

            GLFWwindow* _window;
            static GLFWcursor* _mouseCursors[ImGuiMouseCursor_COUNT];
            UtilityBox::Logger::LoggingSystem _loggingSystem { "ImGui Overhead" };
            const char* _glslVersion;
            static bool _mouseButtonStates[ImGuiMouseButton_COUNT]; // Button input flags.

            std::pair<int, int> _displayDimensions;     // Display width and height values : (width, height)
            std::pair<int, int> _frameBufferDimensions; // Frame buffer width and height values : (width, height)
            std::pair<double, double> _mousePosition;   // Mouse position : (x, y)
    };

    bool OpenGLImGuiOverhead::ImGuiOverheadData::_mouseButtonStates[ImGuiMouseButton_COUNT] {};
    GLFWcursor* OpenGLImGuiOverhead::ImGuiOverheadData::_mouseCursors[ImGuiMouseCursor_COUNT] {};

    OpenGLImGuiOverhead::ImGuiOverheadData::ImGuiOverheadData(GLFWwindow *window, const char *glslVersion) : _window(window),
                                                                                                             _glslVersion(glslVersion) {
        UtilityBox::Logger::LogMessage message {};
        IMGUI_CHECKVERSION();

        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendPlatformName = "GLFW";
        ImGui::StyleColorsDark();

        SetupImGuiOpenGLEnvironment(message);

        for (auto& _mouseCursor : _mouseCursors) {
            _mouseCursor = nullptr;
        }

        for (auto& _mouseButtonState : _mouseButtonStates) {
            _mouseButtonState = false;
        }

        SetupCallbacks();
        SetupKeyMap();
        SetupMouseCursors();
    }

    void OpenGLImGuiOverhead::ImGuiOverheadData::StartFrame() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function StartFrame for OpenGL ImGui overhead.");

        GLFWNewFrame(message);
        ImGui::NewFrame();
    }

    void OpenGLImGuiOverhead::ImGuiOverheadData::RenderFrame() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void OpenGLImGuiOverhead::ImGuiOverheadData::EndFrame() {
        // Nothing to do here.
    }

    void OpenGLImGuiOverhead::ImGuiOverheadData::SetupImGuiOpenGLEnvironment(UtilityBox::Logger::LogMessage& message) {
        message.Supply("Initializing OpenGL environment for ImGui.");
        ImGui_ImplOpenGL3_Init(_glslVersion);
        ImGui_ImplOpenGL3_NewFrame();

        // Taken from ImGui_ImplGlfw_NewFrame(); in file imgui_impl_glfw.cpp (imgui example for GLFW).
        ImGuiIO& io = ImGui::GetIO();
        if (!io.Fonts->IsBuilt()) {
            message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::SEVERE);
            message.Supply("Exception thrown: ImGui font atlas not built - missing/failed call to ImGui_ImplOpenGL3_NewFrame() (renderer back-end).");
            _loggingSystem.Log(message);

            throw std::runtime_error("ImGui font atlas not built - missing call to ImGui_ImplOpenGL3_NewFrame() (renderer back-end).");
        }
    }

    void OpenGLImGuiOverhead::ImGuiOverheadData::GLFWNewFrame(UtilityBox::Logger::LogMessage &message) {
        // Update display / framebuffer size every frame to accommodate for changing window sizes.
        UpdateWindowSize();

        // Update mouse details.
        UpdateMousePosition();
        UpdateMouseCursor();
    }

    void OpenGLImGuiOverhead::ImGuiOverheadData::UpdateWindowSize() {
        ImGuiIO& io = ImGui::GetIO();
        glfwGetWindowSize(_window, &_displayDimensions.first, &_displayDimensions.second);
        glfwGetFramebufferSize(_window, &_frameBufferDimensions.first, &_frameBufferDimensions.second);

        io.DisplaySize = ImVec2((float)_displayDimensions.first, (float)_displayDimensions.second);
        if (_displayDimensions.first > 0 && _displayDimensions.second > 0) {
            io.DisplayFramebufferScale = ImVec2((float)_frameBufferDimensions.first / (float)_displayDimensions.first, (float)_frameBufferDimensions.second / (float)_displayDimensions.second);
        }
    }

    void OpenGLImGuiOverhead::ImGuiOverheadData::SetupCallbacks() {
        glfwSetMouseButtonCallback(_window, GLFWMouseButtonCallback);
        glfwSetScrollCallback(_window, GLFWMouseScrollCallback);
        glfwSetKeyCallback(_window, GLFWKeyCallback);

        ImGuiIO& io = ImGui::GetIO();
        io.SetClipboardTextFn = GLFWSetClipboardText;
        io.GetClipboardTextFn = GLFWGetClipboardText;
        io.ClipboardUserData = _window;
    }

    void OpenGLImGuiOverhead::ImGuiOverheadData::SetupKeyMap() {
        ImGuiIO& io = ImGui::GetIO();

        // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;

        // Keyboard layout.
        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;
    }

    void OpenGLImGuiOverhead::ImGuiOverheadData::SetupMouseCursors() {
        _mouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
        _mouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
        _mouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
        _mouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
        _mouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

    #if GLFW_VERSION_HAS_NEW_CURSORS
        _mouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
        _mouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
        _mouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
        _mouseCursors[ImGuiMouseCursor_NotAllowed] = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);
    #else
        _mouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
        _mouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
        _mouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
        _mouseCursors[ImGuiMouseCursor_NotAllowed] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    #endif
    }

    void OpenGLImGuiOverhead::ImGuiOverheadData::GLFWMouseButtonCallback(GLFWwindow* /* WINDOW UNUSED */, int mouseButton, int buttonAction, int /* MODS UNUSED */) {
        static UtilityBox::Logger::LoggingSystem mouseButtonCallbackLoggingSystem { "GLFW Mouse Callback" }; // Static logging system.

        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entered GLFW mouse button callback function.");

        // Check pressed mouse button range.
        if (mouseButton >= 0 && mouseButton < ImGuiMouseButton_COUNT) {

            // Get human-readable information from the button.
            ButtonIDToString(message, mouseButton);
            ButtonActionToString(message, buttonAction);

            // Handle mouse input.
            switch (buttonAction) {
                case GLFW_PRESS:
                    _mouseButtonStates[mouseButton] = true;
                    break;
                case GLFW_RELEASE:
                    _mouseButtonStates[mouseButton] = false;
                    break;
                default:
                    break;
            }
        }
        else {
            message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::WARNING);
            message.Supply("Pressed mouse button is not a recognized mouse button. Input may have not been processed properly.");
        }

        mouseButtonCallbackLoggingSystem.Log(message);
    }

    void OpenGLImGuiOverhead::ImGuiOverheadData::GLFWMouseScrollCallback(GLFWwindow* /* WINDOW UNUSED */, double xOffset, double yOffset) {
        static UtilityBox::Logger::LoggingSystem mouseScrollCallbackLoggingSystem { "GLFW Key Callback" };
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entered GLFW mouse scroll callback function.");
        message.Supply("Scroll wheel usage detected: horizontal: %f, vertical: %f.", xOffset, yOffset);

        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheelH += (float) xOffset;
        io.MouseWheel += (float) yOffset;

        mouseScrollCallbackLoggingSystem.Log(message);
    }

    void OpenGLImGuiOverhead::ImGuiOverheadData::GLFWKeyCallback(GLFWwindow* /* WINDOW UNUSED */, int keyCode, int /* SCANCODE UNUSED */, int keyAction, int /* MODS UNUSED */) {
        static UtilityBox::Logger::LoggingSystem keyCallbackLoggingSystem { "GLFW Key Callback" };
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function GLFWKeyCallback.");
        KeyStateToString(message, keyCode, keyAction);

        ImGuiIO& io = ImGui::GetIO();
        switch (keyAction) {
            case GLFW_PRESS:
            case GLFW_REPEAT:
                io.KeysDown[keyCode] = true;
                break;
            case GLFW_RELEASE:
                io.KeysDown[keyCode] = false;
                break;
            // Default case has already been handled in message processing - no need to supply extra information to message.
            default:
                break;
        }

        keyCallbackLoggingSystem.Log(message);
    }

    const char *OpenGLImGuiOverhead::ImGuiOverheadData::GLFWGetClipboardText(void *window) {
        static UtilityBox::Logger::LoggingSystem clipboardCallbackLoggingSystem { "GLFW CLipboard Get Text Callback" };
        UtilityBox::Logger::LogMessage message {};

        const char* text = glfwGetClipboardString(static_cast<GLFWwindow*>(window));
        message.Supply("Retrieved string: '%s' from clipboard.", text);
        clipboardCallbackLoggingSystem.Log(message);

        return text;
    }

    void OpenGLImGuiOverhead::ImGuiOverheadData::GLFWSetClipboardText(void *window, const char *text) {
        static UtilityBox::Logger::LoggingSystem clipboardCallbackLoggingSystem { "GLFW CLipboard Set Text Callback" };
        UtilityBox::Logger::LogMessage message {};

        message.Supply("Copied string: '%s' to clipboard.", text);
        glfwSetClipboardString(static_cast<GLFWwindow*>(window), text);
        clipboardCallbackLoggingSystem.Log(message);
    }

    OpenGLImGuiOverhead::ImGuiOverheadData::~ImGuiOverheadData() {
        ImGui_ImplOpenGL3_Shutdown();

        glfwSetMouseButtonCallback(_window, nullptr);
        glfwSetScrollCallback(_window, nullptr);
        glfwSetKeyCallback(_window, nullptr);

        ImGuiIO& io = ImGui::GetIO();
        io.SetClipboardTextFn = nullptr;
        io.GetClipboardTextFn = nullptr;
        io.ClipboardUserData = nullptr;

        for (auto& _mouseCursor : _mouseCursors) {
            glfwDestroyCursor(_mouseCursor);
            _mouseCursor = nullptr;
        }

        // Window deletion is not handled here.
        _window = nullptr;

        ImGui::DestroyContext();
    }

    void OpenGLImGuiOverhead::ImGuiOverheadData::ButtonIDToString(UtilityBox::Logger::LogMessage& message, int mouseButton) {
        switch (mouseButton) {
            // Left mouse button.
            case GLFW_MOUSE_BUTTON_1:
                message.Supply("Mouse button %i pressed. (GLFW_MOUSE_BUTTON_LEFT)", mouseButton);
                break;
            // Right mouse button.
            case GLFW_MOUSE_BUTTON_2:
                message.Supply("Mouse button %i pressed. (GLFW_MOUSE_BUTTON_RIGHT)", mouseButton);
                break;
            // Middle mouse button/
            case GLFW_MOUSE_BUTTON_3:
                message.Supply("Mouse button %i pressed. (GLFW_MOUSE_BUTTON_MIDDLE)", mouseButton);
                break;
            case GLFW_MOUSE_BUTTON_4:
                message.Supply("Mouse button %i pressed. (GLFW_MOUSE_BUTTON_4)", mouseButton);
                break;
            case GLFW_MOUSE_BUTTON_5:
                message.Supply("Mouse button %i pressed. (GLFW_MOUSE_BUTTON_5)", mouseButton);
                break;
            case GLFW_MOUSE_BUTTON_6:
                message.Supply("Mouse button %i pressed. (GLFW_MOUSE_BUTTON_6)", mouseButton);
                break;
            case GLFW_MOUSE_BUTTON_7:
                message.Supply("Mouse button %i pressed. (GLFW_MOUSE_BUTTON_7)", mouseButton);
                break;
            case GLFW_MOUSE_BUTTON_8:
                message.Supply("Mouse button %i pressed. (GLFW_MOUSE_BUTTON_8)", mouseButton);
                break;
            // Should never happen.
            default:
                message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::WARNING);
                message.Supply("Unknown mouse button pressed. Button ID: %i", mouseButton);
                break;
        }
    }

    void OpenGLImGuiOverhead::ImGuiOverheadData::ButtonActionToString(UtilityBox::Logger::LogMessage& message, int buttonAction) {
        // Action can either be GLFW_PRESS or GLFW_RELEASE.
        switch (buttonAction) {
            case GLFW_PRESS:
                message.Supply("Button action: GLFW_PRESS (numerical code: %i).", buttonAction);
                break;
            case GLFW_RELEASE:
                message.Supply("Button action: GLFW_RELEASE (numerical code: %i).", buttonAction);
                break;
            // Should never happen.
            default:
                message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::WARNING);
                message.Supply("Unknown mouse button action. Action ID: %i", buttonAction);
                break;
        }
    }

    void OpenGLImGuiOverhead::ImGuiOverheadData::KeyStateToString(UtilityBox::Logger::LogMessage &message, int keyCode, int keyAction) {
        switch (keyAction) {
            case GLFW_PRESS:
                KeyButtonToString(message, keyCode, "pressed");
                break;
            case GLFW_RELEASE:
                KeyButtonToString(message, keyCode, "released");
                break;
            case GLFW_REPEAT:
                KeyButtonToString(message, keyCode, "held down");
                break;
                // Should never happen.
            default:
                message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::WARNING);
                message.Supply("Unknown key action (action code: %i)", keyAction);
                break;
        }
    }

    void OpenGLImGuiOverhead::ImGuiOverheadData::KeyButtonToString(UtilityBox::Logger::LogMessage &message, int keyCode, const char *actionString) {
        switch (keyCode) {
            case GLFW_KEY_SPACE:
                message.Supply("Key %s: GLFW_KEY_SPACE (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_APOSTROPHE:
                message.Supply("Key %s: GLFW_KEY_APOSTROPHE (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_COMMA:
                message.Supply("Key %s: GLFW_KEY_COMMA (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_MINUS:
                message.Supply("Key %s: GLFW_KEY_MINUS (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_PERIOD:
                message.Supply("Key %s: GLFW_KEY_PERIOD (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_SLASH:
                message.Supply("Key %s: GLFW_KEY_SLASH (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_0:
                message.Supply("Key %s: GLFW_KEY_0 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_1:
                message.Supply("Key %s: GLFW_KEY_1 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_2:
                message.Supply("Key %s: GLFW_KEY_2 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_3:
                message.Supply("Key %s: GLFW_KEY_3 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_4:
                message.Supply("Key %s: GLFW_KEY_4 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_5:
                message.Supply("Key %s: GLFW_KEY_5 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_6:
                message.Supply("Key %s: GLFW_KEY_6 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_7:
                message.Supply("Key %s: GLFW_KEY_7 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_8:
                message.Supply("Key %s: GLFW_KEY_8 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_9:
                message.Supply("Key %s: GLFW_KEY_9 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_SEMICOLON:
                message.Supply("Key %s: GLFW_KEY_SEMICOLON (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_EQUAL:
                message.Supply("Key %s: GLFW_KEY_EQUAL (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_A:
                message.Supply("Key %s: GLFW_KEY_A (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_B:
                message.Supply("Key %s: GLFW_KEY_B (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_C:
                message.Supply("Key %s: GLFW_KEY_C (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_D:
                message.Supply("Key %s: GLFW_KEY_D (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_E:
                message.Supply("Key %s: GLFW_KEY_E (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F:
                message.Supply("Key %s: GLFW_KEY_F (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_G:
                message.Supply("Key %s: GLFW_KEY_G (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_H:
                message.Supply("Key %s: GLFW_KEY_H (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_I:
                message.Supply("Key %s: GLFW_KEY_I (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_J:
                message.Supply("Key %s: GLFW_KEY_J (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_K:
                message.Supply("Key %s: GLFW_KEY_K (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_L:
                message.Supply("Key %s: GLFW_KEY_L (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_M:
                message.Supply("Key %s: GLFW_KEY_M (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_N:
                message.Supply("Key %s: GLFW_KEY_N (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_O:
                message.Supply("Key %s: GLFW_KEY_O (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_P:
                message.Supply("Key %s: GLFW_KEY_P (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_Q:
                message.Supply("Key %s: GLFW_KEY_Q (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_R:
                message.Supply("Key %s: GLFW_KEY_R (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_S:
                message.Supply("Key %s: GLFW_KEY_S (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_T:
                message.Supply("Key %s: GLFW_KEY_T (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_U:
                message.Supply("Key %s: GLFW_KEY_U (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_V:
                message.Supply("Key %s: GLFW_KEY_V (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_W:
                message.Supply("Key %s: GLFW_KEY_W (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_X:
                message.Supply("Key %s: GLFW_KEY_X (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_Y:
                message.Supply("Key %s: GLFW_KEY_Y (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_Z:
                message.Supply("Key %s: GLFW_KEY_Z (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_LEFT_BRACKET:
                message.Supply("Key %s: GLFW_KEY_LEFT_BRACKET (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_BACKSLASH:
                message.Supply("Key %s: GLFW_KEY_BACKSLASH (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_RIGHT_BRACKET:
                message.Supply("Key %s: GLFW_KEY_RIGHT_BRACKET (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_GRAVE_ACCENT:
                message.Supply("Key %s: GLFW_KEY_GRAVE_ACCENT (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_ESCAPE:
                message.Supply("Key %s: GLFW_KEY_ESCAPE (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_ENTER:
                message.Supply("Key %s: GLFW_KEY_ENTER (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_TAB:
                message.Supply("Key %s: GLFW_KEY_TAB (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_BACKSPACE:
                message.Supply("Key %s: GLFW_KEY_BACKSPACE (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_INSERT:
                message.Supply("Key %s: GLFW_KEY_INSERT (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_DELETE:
                message.Supply("Key %s: GLFW_KEY_DELETE (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_RIGHT:
                message.Supply("Key %s: GLFW_KEY_RIGHT (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_LEFT:
                message.Supply("Key %s: GLFW_KEY_LEFT (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_DOWN:
                message.Supply("Key %s: GLFW_KEY_DOWN (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_UP:
                message.Supply("Key %s: GLFW_KEY_UP (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_PAGE_UP:
                message.Supply("Key %s: GLFW_KEY_PAGE_UP (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_PAGE_DOWN:
                message.Supply("Key %s: GLFW_KEY_PAGE_DOWN (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_HOME:
                message.Supply("Key %s: GLFW_KEY_HOME (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_END:
                message.Supply("Key %s: GLFW_KEY_END (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_CAPS_LOCK:
                message.Supply("Key %s: GLFW_KEY_CAPS_LOCK (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_SCROLL_LOCK:
                message.Supply("Key %s: GLFW_KEY_SCROLL_LOCK (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_NUM_LOCK:
                message.Supply("Key %s: GLFW_KEY_NUM_LOCK (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_PRINT_SCREEN:
                message.Supply("Key %s: GLFW_KEY_PRINT_SCREEN (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_PAUSE:
                message.Supply("Key %s: GLFW_KEY_PAUSE (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F1:
                message.Supply("Key %s: GLFW_KEY_F1 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F2:
                message.Supply("Key %s: GLFW_KEY_F2 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F3:
                message.Supply("Key %s: GLFW_KEY_F3 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F4:
                message.Supply("Key %s: GLFW_KEY_F4 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F5:
                message.Supply("Key %s: GLFW_KEY_F5 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F6:
                message.Supply("Key %s: GLFW_KEY_F6 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F7:
                message.Supply("Key %s: GLFW_KEY_F7 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F8:
                message.Supply("Key %s: GLFW_KEY_F8 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F9:
                message.Supply("Key %s: GLFW_KEY_F9 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F10:
                message.Supply("Key %s: GLFW_KEY_F10 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F11:
                message.Supply("Key %s: GLFW_KEY_F11 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F12:
                message.Supply("Key %s: GLFW_KEY_F12 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F13:
                message.Supply("Key %s: GLFW_KEY_F13 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F14:
                message.Supply("Key %s: GLFW_KEY_F14 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F15:
                message.Supply("Key %s: GLFW_KEY_F15 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F16:
                message.Supply("Key %s: GLFW_KEY_F16 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F17:
                message.Supply("Key %s: GLFW_KEY_F17 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F18:
                message.Supply("Key %s: GLFW_KEY_F18 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F19:
                message.Supply("Key %s: GLFW_KEY_F19 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F20:
                message.Supply("Key %s: GLFW_KEY_F20 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F21:
                message.Supply("Key %s: GLFW_KEY_F21 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F22:
                message.Supply("Key %s: GLFW_KEY_F22 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F23:
                message.Supply("Key %s: GLFW_KEY_F23 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F24:
                message.Supply("Key %s: GLFW_KEY_F24 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_F25:
                message.Supply("Key %s: GLFW_KEY_F25 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_KP_0:
                message.Supply("Key %s: GLFW_KEY_KP_0 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_KP_1:
                message.Supply("Key %s: GLFW_KEY_KP_1 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_KP_2:
                message.Supply("Key %s: GLFW_KEY_KP_2 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_KP_3:
                message.Supply("Key %s: GLFW_KEY_KP_3 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_KP_4:
                message.Supply("Key %s: GLFW_KEY_KP_4 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_KP_5:
                message.Supply("Key %s: GLFW_KEY_KP_5 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_KP_6:
                message.Supply("Key %s: GLFW_KEY_KP_6 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_KP_7:
                message.Supply("Key %s: GLFW_KEY_KP_7 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_KP_8:
                message.Supply("Key %s: GLFW_KEY_KP_8 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_KP_9:
                message.Supply("Key %s: GLFW_KEY_KP_9 (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_KP_DECIMAL:
                message.Supply("Key %s: GLFW_KEY_KP_DECIMAL (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_KP_DIVIDE:
                message.Supply("Key %s: GLFW_KEY_KP_DIVIDE (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_KP_MULTIPLY:
                message.Supply("Key %s: GLFW_KEY_KP_MULTIPLY (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_KP_SUBTRACT:
                message.Supply("Key %s: GLFW_KEY_KP_SUBTRACT (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_KP_ADD:
                message.Supply("Key %s: GLFW_KEY_KP_ADD (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_KP_ENTER:
                message.Supply("Key %s: GLFW_KEY_KP_ENTER (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_KP_EQUAL:
                message.Supply("Key %s: GLFW_KEY_KP_EQUAL (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_LEFT_SHIFT:
                message.Supply("Key %s: GLFW_KEY_LEFT_SHIFT (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_LEFT_CONTROL:
                message.Supply("Key %s: GLFW_KEY_LEFT_CONTROL (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_LEFT_ALT:
                message.Supply("Key %s: GLFW_KEY_LEFT_ALT (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_LEFT_SUPER:
                message.Supply("Key %s: GLFW_KEY_LEFT_SUPER (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_RIGHT_SHIFT:
                message.Supply("Key %s: GLFW_KEY_RIGHT_SHIFT (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_RIGHT_CONTROL:
                message.Supply("Key %s: GLFW_KEY_RIGHT_CONTROL (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_RIGHT_ALT:
                message.Supply("Key %s: GLFW_KEY_RIGHT_ALT (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_RIGHT_SUPER:
                message.Supply("Key %s: GLFW_KEY_RIGHT_SUPER (key code: %i)", actionString, keyCode);
                break;
            case GLFW_KEY_MENU:
                message.Supply("Key %s: GLFW_KEY_MENU (key code: %i)", actionString, keyCode);
                break;
            default:
                message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::WARNING);
                message.Supply("Unknown Key %s: GLFW_KEY_UNKNOWN (key code: %i)", actionString, keyCode);
        }
    }

    void OpenGLImGuiOverhead::ImGuiOverheadData::UpdateMousePosition() {
        // Update mouse button state.
        ImGuiIO& io = ImGui::GetIO();
        for (int i = 0; i < ImGuiMouseButton_COUNT; i++) {
            // If a mouse press event came, always pass it as "mouse held this frame", so click-release events that are shorter than 1 frame are not missed.
            io.MouseDown[i] = _mouseButtonStates[i] || glfwGetMouseButton(_window, i) != 0;
            _mouseButtonStates[i] = false;
        }

        // Update mouse position
        ImVec2 previousMousePosition = io.MousePos;
        io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);

        // Update the mouse position if the window is focused.
        if (glfwGetWindowAttrib(_window, GLFW_FOCUSED)) {
            glfwGetCursorPos(_window, &_mousePosition.first, &_mousePosition.second);
            io.MousePos = ImVec2((float)_mousePosition.first, (float)_mousePosition.second);
        }
    }

    void OpenGLImGuiOverhead::ImGuiOverheadData::UpdateMouseCursor() {
        ImGuiIO& io = ImGui::GetIO();
        // If mouse cursor is explicitly set to not change or if the cursor is disabled, don't do any updates.
        if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) || glfwGetInputMode(_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
            return;
        }

        ImGuiMouseCursor currentCursor = ImGui::GetMouseCursor();
        if (currentCursor == ImGuiMouseCursor_None || io.MouseDrawCursor) {
            // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }
        else {
            // Show OS mouse cursor
            glfwSetCursor(_window, _mouseCursors[currentCursor] ? _mouseCursors[currentCursor] : _mouseCursors[ImGuiMouseCursor_Arrow]);
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    OpenGLImGuiOverhead::OpenGLImGuiOverhead(GLFWwindow* window, const char* glslVersion) : _data(new ImGuiOverheadData(window, glslVersion)),
                                                                                            Graphics::Renderer::ImGuiOverhead()
                                                                                            {
        // Nothing to do here.
    }

    OpenGLImGuiOverhead::~OpenGLImGuiOverhead() {
        delete _data;
    }

    void OpenGLImGuiOverhead::StartFrame() {
        _data->StartFrame();
    }

    void OpenGLImGuiOverhead::RenderFrame() {
        _data->RenderFrame();
    }

    void OpenGLImGuiOverhead::EndFrame() {
        _data->EndFrame();
    }
}
