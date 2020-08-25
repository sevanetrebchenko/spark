
#include <imgui.h>
#include <examples/imgui_impl_opengl3.h>                 // ImGui_ImplOpenGL3_Init, ImGui_ImplOpenGL3_NewFrame, ImGui_ImplOpenGL3_RenderDrawData
#include <platform/opengl/renderer/opengl_imgui_layer.h> // OpenGLImGuiLayer
#include <spark/core/service_locator.h>                  // ServiceLocator
#include <spark/events/types/mouse_events.h>             // MouseMovedEvent, MouseScrolledEvent
#include <spark/events/event_interactable_interface.h>   // IEventReceivable

namespace Spark::Graphics {
    //------------------------------------------------------------------------------------------------------------------
    // OPENGL IMGUI LAYER DATA
    //------------------------------------------------------------------------------------------------------------------
    class OpenGLImGuiLayer::OpenGLImGuiLayerData : public Events::IEventReceivable<OpenGLImGuiLayerData, Events::MouseScrolledEvent> {
        public:
            explicit OpenGLImGuiLayerData(GLFWwindow* window);
            ~OpenGLImGuiLayerData();

            void OnAttach();
            void OnDetach();
            void OnUpdate(float dt);
            void OnImGuiRender();

            void BeginFrame();
            void EndFrame();

        private:
            friend class Events::IEventReceivable<OpenGLImGuiLayerData, Events::MouseScrolledEvent>;
            void OnEvent(Events::MouseScrolledEvent* event) override;

            void SetupImGuiOpenGLEnvironment();
            void SetupGLFWCallbacks();
            void SetupKeyMap();
            void SetupMouseCursors();

            void UpdateWindowSize();
            void UpdateMouseState();
            void UpdateMouseCursor();

            GLFWwindow* _window;

            GLFWcursor* _mouseCursors[ImGuiMouseCursor_COUNT] { nullptr }; // Various ImGui mouse cursors.
            bool _mouseButtonStates[ImGuiMouseButton_COUNT] { false };   // Button input flags.

            std::pair<int, int> _displayDimensions;     // Display width and height values : (width, height)
            std::pair<int, int> _frameBufferDimensions; // Frame buffer width and height values : (width, height)
            std::pair<double, double> _mousePosition;   // Mouse position : (x, y)
    };

    OpenGLImGuiLayer::OpenGLImGuiLayerData::OpenGLImGuiLayerData(GLFWwindow* window) : Events::IEventReceivable<OpenGLImGuiLayerData, Events::MouseScrolledEvent>("OpenGL ImGui Layer"),
                                                                                       _window(window) {
        SP_ASSERT(window, "Window is null");
    }

    // Void GLFW callback functions and clean up ImGui state.
    OpenGLImGuiLayer::OpenGLImGuiLayerData::~OpenGLImGuiLayerData() {

    }

    void OpenGLImGuiLayer::OpenGLImGuiLayerData::BeginFrame() {
        // OnUpdate display / framebuffer size every frame to accommodate for changing window sizes.
        UpdateWindowSize();

        // OnUpdate mouse details.
        UpdateMouseState();
        UpdateMouseCursor();

        ImGui::NewFrame();
    }

    void OpenGLImGuiLayer::OpenGLImGuiLayerData::EndFrame() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void OpenGLImGuiLayer::OpenGLImGuiLayerData::OnEvent(Events::MouseScrolledEvent *event) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheelH += (float)event->GetScrollX();
        io.MouseWheel += (float)event->GetScrollY();
    }

    // Initialize back-end ImGui renderer and environment.
    void OpenGLImGuiLayer::OpenGLImGuiLayerData::SetupImGuiOpenGLEnvironment() {
        // Back-end ImGui renderer.
        ImGui_ImplOpenGL3_Init();
        ImGui_ImplOpenGL3_NewFrame();

        // Taken from ImGui_ImplGlfw_NewFrame(), from file imgui_impl_glfw.cpp (imgui example for GLFW).
        ImGuiIO& io = ImGui::GetIO();
        if (!io.Fonts->IsBuilt()) {
            throw std::runtime_error("ImGui font atlas not built - missing call to ImGui_ImplOpenGL3_NewFrame() (renderer back-end).");
        }
    }

    void OpenGLImGuiLayer::OpenGLImGuiLayerData::SetupGLFWCallbacks() {
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
    void OpenGLImGuiLayer::OpenGLImGuiLayerData::SetupKeyMap() {
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
    void OpenGLImGuiLayer::OpenGLImGuiLayerData::SetupMouseCursors() {
        _mouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
        _mouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
        _mouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
        _mouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
        _mouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

#ifdef GLFW_VERSION_HAS_NEW_CURSORS
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

    // OnUpdate window size and framebuffer in case window was resized since the last frame.
    void OpenGLImGuiLayer::OpenGLImGuiLayerData::UpdateWindowSize() {
        ImGuiIO& io = ImGui::GetIO();
        glfwGetWindowSize(_window, &_displayDimensions.first, &_displayDimensions.second);
        glfwGetFramebufferSize(_window, &_frameBufferDimensions.first, &_frameBufferDimensions.second);

        io.DisplaySize = ImVec2((float)_displayDimensions.first, (float)_displayDimensions.second);
        if (_displayDimensions.first > 0 && _displayDimensions.second > 0) {
            io.DisplayFramebufferScale = ImVec2((float)_frameBufferDimensions.first / (float)_displayDimensions.first, (float)_frameBufferDimensions.second / (float)_displayDimensions.second);
        }
    }

    // Updates position of the mouse.
    void OpenGLImGuiLayer::OpenGLImGuiLayerData::UpdateMouseState() {
        // OnUpdate mouse button state.
        ImGuiIO& io = ImGui::GetIO();
        for (int i = 0; i < ImGuiMouseButton_COUNT; i++) {
            // If a mouse press event came, always pass it as "mouse held this frame", so click-release events that are shorter than 1 frame are not missed.
            io.MouseDown[i] = _mouseButtonStates[i] || glfwGetMouseButton(_window, i) != 0;
            _mouseButtonStates[i] = false;
        }

        // OnUpdate the mouse position if the window is focused.
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

    // OnUpdate the mouse cursor based on the mouse position relative to the ImGui windows.
    void OpenGLImGuiLayer::OpenGLImGuiLayerData::UpdateMouseCursor() {
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

    void OpenGLImGuiLayer::OpenGLImGuiLayerData::OnAttach() {
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

    void OpenGLImGuiLayer::OpenGLImGuiLayerData::OnDetach() {
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

    void OpenGLImGuiLayer::OpenGLImGuiLayerData::OnUpdate(float) {

    }

    void OpenGLImGuiLayer::OpenGLImGuiLayerData::OnImGuiRender() {

    }


    //------------------------------------------------------------------------------------------------------------------
    // OPENGL IMGUI LAYER
    //------------------------------------------------------------------------------------------------------------------
    OpenGLImGuiLayer::OpenGLImGuiLayer(GLFWwindow* window) : _data(new OpenGLImGuiLayerData(window)) {
        // Nothing to do here.
    }

    OpenGLImGuiLayer::~OpenGLImGuiLayer() {
        delete _data;
    }

    void OpenGLImGuiLayer::OnAttach() {
        _data->OnAttach();
    }

    void OpenGLImGuiLayer::OnDetach() {
        _data->OnDetach();
    }

    void OpenGLImGuiLayer::OnUpdate(float dt) {
        _data->OnUpdate(dt);
    }

    void OpenGLImGuiLayer::OnImGuiRender() {
        _data->OnImGuiRender();
    }

    void OpenGLImGuiLayer::BeginFrame() {
        _data->BeginFrame();
    }

    void OpenGLImGuiLayer::EndFrame() {
        _data->EndFrame();
    }
}
