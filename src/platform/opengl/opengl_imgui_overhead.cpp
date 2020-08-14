
#include <platform/opengl/opengl_imgui_overhead.h>
#include <utilitybox/logger/logging_system.h>
#include <events/mouse_events.h>
#include <events/key_events.h>
#include <core/service_locator.h>
#include <examples/imgui_impl_opengl3.h>           // ImGui_ImplOpenGL3_Init, ImGui_ImplOpenGL3_NewFrame, ImGui_ImplOpenGL3_RenderDrawData
#include <imgui.h>

#define GLFW_VERSION_HAS_NEW_CURSORS (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3400)

namespace Spark::Graphics {

    class OpenGLImGuiOverhead::OpenGLImGuiOverheadData {
        public:
            explicit OpenGLImGuiOverheadData(GLFWwindow* window);
            ~OpenGLImGuiOverheadData();
            void BeginFrame();
            void EndFrame();

        private:
            void SetupImGuiOpenGLEnvironment();
            void SetupGLFWCallbacks();
            void SetupKeyMap();
            void SetupMouseCursors();

            void GLFWNewFrame();
            void UpdateWindowSize();
            void UpdateMouseState();
            void UpdateMouseCursor();

            GLFWwindow* _window;
            UtilityBox::Logger::LoggingSystem _loggingSystem;

            GLFWcursor* _mouseCursors[ImGuiMouseCursor_COUNT] { nullptr }; // Various ImGui mouse cursors.
            bool _mouseButtonStates[ImGuiMouseButton_COUNT] { false };   // Button input flags.

            std::pair<int, int> _displayDimensions;     // Display width and height values : (width, height)
            std::pair<int, int> _frameBufferDimensions; // Frame buffer width and height values : (width, height)
            std::pair<double, double> _mousePosition;   // Mouse position : (x, y)
    };

    OpenGLImGuiOverhead::OpenGLImGuiOverheadData::OpenGLImGuiOverheadData(GLFWwindow* window) : _window(window) {
        IMGUI_CHECKVERSION();

        // Context settings and preferences.
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.BackendPlatformName = "GLFW";
        ImGui::StyleColorsDark();

        SetupImGuiOpenGLEnvironment();
        SetupGLFWCallbacks();
        SetupKeyMap();
        SetupMouseCursors();
    }

    // Void GLFW callback functions and clean up ImGui state.
    OpenGLImGuiOverhead::OpenGLImGuiOverheadData::~OpenGLImGuiOverheadData() {
        ImGui_ImplOpenGL3_Shutdown();

        // Clear callbacks.
        glfwSetMouseButtonCallback(_window, nullptr);
        glfwSetScrollCallback(_window, nullptr);
        glfwSetKeyCallback(_window, nullptr);

        ImGuiIO& io = ImGui::GetIO();
        io.SetClipboardTextFn = nullptr;
        io.GetClipboardTextFn = nullptr;
        io.ClipboardUserData = nullptr;

        // Destroy cursors.
        for (auto& _mouseCursor : _mouseCursors) {
            glfwDestroyCursor(_mouseCursor);
            _mouseCursor = nullptr;
        }

        // Window deletion is not handled here.
        _window = nullptr;

        ImGui::DestroyContext();
    }

    void OpenGLImGuiOverhead::OpenGLImGuiOverheadData::BeginFrame() {
        GLFWNewFrame();
        ImGui::NewFrame();
    }

    void OpenGLImGuiOverhead::OpenGLImGuiOverheadData::EndFrame() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    // Initialize back-end ImGui renderer and environment.
    void OpenGLImGuiOverhead::OpenGLImGuiOverheadData::SetupImGuiOpenGLEnvironment() {
        // Back-end ImGui renderer.
        ImGui_ImplOpenGL3_Init();
        ImGui_ImplOpenGL3_NewFrame();

        // Taken from ImGui_ImplGlfw_NewFrame(), from file imgui_impl_glfw.cpp (imgui example for GLFW).
        ImGuiIO& io = ImGui::GetIO();
        if (!io.Fonts->IsBuilt()) {
            _loggingSystem.Log(UtilityBox::Logger::LogMessageSeverity::SEVERE, "Exception thrown: ImGui font atlas not built - missing/failed call to ImGui_ImplOpenGL3_NewFrame() (renderer back-end).");
            throw std::runtime_error("ImGui font atlas not built - missing call to ImGui_ImplOpenGL3_NewFrame() (renderer back-end).");
        }

        // Initialize mouse cursors to null.
        for (auto& _mouseCursor : _mouseCursors) {
            _mouseCursor = nullptr;
        }

        // Initialize mouse button states to default.
        for (auto& _mouseButtonState : _mouseButtonStates) {
            _mouseButtonState = false;
        }
    }

    void OpenGLImGuiOverhead::OpenGLImGuiOverheadData::SetupGLFWCallbacks() {
        // Mouse button callback.
        glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int mouseButton, int buttonAction, int /* MODS UNUSED */) {
            if (buttonAction == GLFW_PRESS) {
                ServiceLocator::GetEventHub()->Dispatch(new Events::MouseButtonPressedEvent(mouseButton));
            }
            else if (buttonAction == GLFW_RELEASE) {
                ServiceLocator::GetEventHub()->Dispatch(new Events::MouseButtonReleasedEvent(mouseButton));
            }
        });

        // Mouse scroll wheel callback.
        glfwSetScrollCallback(_window, [](GLFWwindow* /* WINDOW UNUSED */, double xOffset, double yOffset) {
            ServiceLocator::GetEventHub()->Dispatch(new Events::MouseScrolledEvent(xOffset, yOffset));
        });

        // Key callback.
        glfwSetKeyCallback(_window, [](GLFWwindow* /* WINDOW UNUSED */, int keyCode, int /* SCANCODE UNUSED */, int keyAction, int /* MODS UNUSED */) {
            if (keyAction == GLFW_PRESS) {
                ServiceLocator::GetEventHub()->Dispatch(new Events::KeyPressEvent(keyCode));
            }
            else if (keyAction == GLFW_RELEASE) {
                ServiceLocator::GetEventHub()->Dispatch(new Events::KeyReleaseEvent(keyCode));
            }
        });

        // Clipboard get/set functions.
        ImGuiIO& io = ImGui::GetIO();
        io.SetClipboardTextFn = [](void* window, const char* text) {
            glfwSetClipboardString(static_cast<GLFWwindow*>(window), text);
        };
        io.GetClipboardTextFn = [](void* window) -> const char* {
            return glfwGetClipboardString(static_cast<GLFWwindow*>(window));
        };
        io.ClipboardUserData = _window;
    }

    // Initialize ImGui keymap (KeysDown array).
    void OpenGLImGuiOverhead::OpenGLImGuiOverheadData::SetupKeyMap() {
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

    // Initialize various ImGui mouse cursors into the static mouse cursor storage.
    void OpenGLImGuiOverhead::OpenGLImGuiOverheadData::SetupMouseCursors() {
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

    // Initialize a new ImGui frame for GLFW. Function updates mouse details and scales window framebuffer in case the
    // window was resized since the past frame.
    void OpenGLImGuiOverhead::OpenGLImGuiOverheadData::GLFWNewFrame() {
        // Update display / framebuffer size every frame to accommodate for changing window sizes.
        UpdateWindowSize();

        // Update mouse details.
        UpdateMouseState();
        UpdateMouseCursor();
    }

    // Update window size and framebuffer in case window was resized since the last frame.
    void OpenGLImGuiOverhead::OpenGLImGuiOverheadData::UpdateWindowSize() {
        ImGuiIO& io = ImGui::GetIO();
        glfwGetWindowSize(_window, &_displayDimensions.first, &_displayDimensions.second);
        glfwGetFramebufferSize(_window, &_frameBufferDimensions.first, &_frameBufferDimensions.second);

        io.DisplaySize = ImVec2((float)_displayDimensions.first, (float)_displayDimensions.second);
        if (_displayDimensions.first > 0 && _displayDimensions.second > 0) {
            io.DisplayFramebufferScale = ImVec2((float)_frameBufferDimensions.first / (float)_displayDimensions.first, (float)_frameBufferDimensions.second / (float)_displayDimensions.second);
        }
    }

    // Updates position of the mouse.
    void OpenGLImGuiOverhead::OpenGLImGuiOverheadData::UpdateMouseState() {
        // Update mouse button state.
        ImGuiIO& io = ImGui::GetIO();
        for (int i = 0; i < ImGuiMouseButton_COUNT; i++) {
            // If a mouse press event came, always pass it as "mouse held this frame", so click-release events that are shorter than 1 frame are not missed.
            io.MouseDown[i] = _mouseButtonStates[i] || glfwGetMouseButton(_window, i) != 0;
            _mouseButtonStates[i] = false;
        }

        // Update the mouse position if the window is focused.
        if (glfwGetWindowAttrib(_window, GLFW_FOCUSED)) {
            double currentMouseX, currentMouseY;
            glfwGetCursorPos(_window, &currentMouseX, &currentMouseY);

            // Mouse moved.
            if (currentMouseX != _mousePosition.first || currentMouseY != _mousePosition.second) {
                _mousePosition.first = currentMouseX;
                _mousePosition.second = currentMouseY;
                io.MousePos = ImVec2((float)_mousePosition.first, (float)_mousePosition.second);

                ServiceLocator::GetEventHub()->Dispatch(new Events::MouseMovedEvent(_mousePosition.first, _mousePosition.second));
            }
        }
    }

    // Update the mouse cursor based on the mouse position relative to the ImGui windows.
    void OpenGLImGuiOverhead::OpenGLImGuiOverheadData::UpdateMouseCursor() {
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


    //------------------------------------------------------------------------------------------------------------------
    // OPENGL IMGUI OVERHEAD
    //------------------------------------------------------------------------------------------------------------------
    OpenGLImGuiOverhead::OpenGLImGuiOverhead(GLFWwindow* window) : _data(new OpenGLImGuiOverheadData(window)) {
        // Nothing to do here.
    }

    OpenGLImGuiOverhead::~OpenGLImGuiOverhead() {
        delete _data;
    }

    void OpenGLImGuiOverhead::BeginFrame() {
        _data->BeginFrame();
    }

    void OpenGLImGuiOverhead::EndFrame() {
        _data->EndFrame();
    }

}
