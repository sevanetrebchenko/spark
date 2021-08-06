
#include <glad/glad.h>
#include <GLFW/glfw3.h>                                   // GLFWwindow
#include <platform/opengl/window/opengl_window.h>         // OpenGLWindow
#include <spark/utilitybox/logger/logging_interface.h>    // ILoggable
#include <spark/graphics/renderer/rendering_context.h>    // RenderingContext
#include <spark/utilitybox/logger/log_message_severity.h> // LogSeverity
#include <spark/utilitybox/logger/logging_system.h>       // LoggingSystem
#include <spark/core/service_locator.h>                   // ServiceLocator
#include <spark/events/types/key_events.h>                // KeyPressedEvent, KeyReleasedEvent
#include <spark/events/types/mouse_events.h>              // MouseScrolledEvent, MouseButtonPressedEvent, MouseButtonReleasedEvent
#include <spark/events/types/application_events.h>        // WindowCloseEvent
#include <spark/events/event_interactable_interface.h>    // IEventReceivable

namespace Spark::Graphics {
    //------------------------------------------------------------------------------------------------------------------
    // WINDOW DATA
    //------------------------------------------------------------------------------------------------------------------
    class OpenGLWindow::OpenGLWindowData {//}; : public Events::IEventReceivable<OpenGLWindowData, Events::WindowResizeEvent>, public UtilityBox::Logger::ILoggable {
        public:
            OpenGLWindowData(std::string windowName, int width, int height);
            ~OpenGLWindowData();

            void OnUpdate();

            NODISCARD int GetWidth() const;
            NODISCARD int GetHeight() const;
            NODISCARD GLFWwindow* GetNativeWindow() const;

        private:
//            friend class Events::IEventReceivable<OpenGLWindowData, Events::WindowResizeEvent>;
//            void OnEvent(Events::WindowResizeEvent* windowResizeEvent) override;

            void InitializeGLFW();
            void SetupGLFWCallbacks();

            std::string _windowName;
            int _windowWidth, _windowHeight;

            GLFWwindow* _window;
            Graphics::IRenderingContext* _context;
    };

    OpenGLWindow::OpenGLWindowData::OpenGLWindowData(std::string windowName, int width, int height) : _windowName(std::move(windowName)),
                                                                                                      _windowWidth(width),
                                                                                                      _windowHeight(height)
//                                                                                                      IEventReceivable("OpenGL Window"),
//                                                                                                      UtilityBox::Logger::ILoggable("OpenGL Window")
                                                                                                      {
        // GLFW needs to be initialized before creating a GLFW window.
        InitializeGLFW();

        // TODO: Support higher version of OpenGL
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create window.
        _window = glfwCreateWindow(_windowWidth, _windowHeight, _windowName.c_str(), nullptr, nullptr);
        SP_ASSERT(_window != nullptr, "Failed to initialize GLFW window.");

        // Create rendering context.
        _context = Graphics::IRenderingContext::Create(_window);
        SP_ASSERT(_context != nullptr, "Context was not initialized - unsupported graphics API selected.");

        _context->SetClearColor(glm::vec4(0.45f, 0.55f, 0.60f, 1.00f));
        SetupGLFWCallbacks();
    }

    OpenGLWindow::OpenGLWindowData::~OpenGLWindowData() {
        glfwDestroyWindow(static_cast<GLFWwindow*>(_window));
    }

    int OpenGLWindow::OpenGLWindowData::GetWidth() const {
        return _windowWidth;
    }

    int OpenGLWindow::OpenGLWindowData::GetHeight() const {
        return _windowHeight;
    }

    GLFWwindow *OpenGLWindow::OpenGLWindowData::GetNativeWindow() const {
        return _window;
    }

//    void OpenGLWindow::OpenGLWindowData::OnEvent(Events::WindowResizeEvent *windowResizeEvent) {
//        GLint viewportData[4];
//        glGetIntegerv(GL_VIEWPORT, viewportData);
//
//        _windowWidth = windowResizeEvent->GetWidth();
//        _windowHeight = windowResizeEvent->GetHeight();
//
//        _context->SetViewport(viewportData[0], viewportData[1], _windowWidth, _windowHeight);
//    }

    void OpenGLWindow::OpenGLWindowData::InitializeGLFW() {
//        int initializationCode = glfwInit();
//        // Failed to initialize GLFW
//        if (!initializationCode) {
//            LogError("GLFW initialization failed with error code: %i.", initializationCode);
//            throw std::runtime_error("GLFW initialization failed.");
//        }
//        else {
//            LogDebug("GLFW successfully initialized.");
//        }
//
//        glfwSetErrorCallback([](int errorCode, const char *errorDescription) {
//            static UtilityBox::Logger::LoggingSystem errorLoggingSystem("GLFW Error");
//
//            switch (errorCode) {
//                case GLFW_NO_ERROR:
//                    errorLoggingSystem.Log(UtilityBox::Logger::LogSeverity::ERROR, "GLFW error occurred: GLFW_NO_ERROR (error code: %i).", errorCode);
//                    break;
//                case GLFW_NOT_INITIALIZED:
//                    errorLoggingSystem.Log(UtilityBox::Logger::LogSeverity::ERROR, "GLFW error occurred: GLFW_NOT_INITIALIZED (error code: %i).", errorCode);
//                    break;
//                case GLFW_NO_CURRENT_CONTEXT:
//                    errorLoggingSystem.Log(UtilityBox::Logger::LogSeverity::ERROR, "GLFW error occurred: GLFW_NO_CURRENT_CONTEXT (error code: %i).", errorCode);
//                    break;
//                case GLFW_INVALID_ENUM:
//                    errorLoggingSystem.Log(UtilityBox::Logger::LogSeverity::ERROR, "GLFW error occurred: GLFW_INVALID_ENUM (error code: %i).", errorCode);
//                    break;
//                case GLFW_INVALID_VALUE:
//                    errorLoggingSystem.Log(UtilityBox::Logger::LogSeverity::ERROR, "GLFW error occurred: GLFW_INVALID_VALUE (error code: %i).", errorCode);
//                    break;
//                case GLFW_OUT_OF_MEMORY:
//                    errorLoggingSystem.Log(UtilityBox::Logger::LogSeverity::ERROR, "GLFW error occurred: GLFW_OUT_OF_MEMORY (error code: %i).", errorCode);
//                    break;
//                case GLFW_API_UNAVAILABLE:
//                    errorLoggingSystem.Log(UtilityBox::Logger::LogSeverity::ERROR, "GLFW error occurred: GLFW_API_UNAVAILABLE (error code: %i).", errorCode);
//                    break;
//                case GLFW_VERSION_UNAVAILABLE:
//                    errorLoggingSystem.Log(UtilityBox::Logger::LogSeverity::ERROR, "GLFW error occurred: GLFW_VERSION_UNAVAILABLE (error code: %i).", errorCode);
//                    break;
//                case GLFW_PLATFORM_ERROR:
//                    errorLoggingSystem.Log(UtilityBox::Logger::LogSeverity::ERROR, "GLFW error occurred: GLFW_PLATFORM_ERROR (error code: %i).", errorCode);
//                    break;
//                case GLFW_FORMAT_UNAVAILABLE:
//                    errorLoggingSystem.Log(UtilityBox::Logger::LogSeverity::ERROR, "GLFW error occurred: GLFW_FORMAT_UNAVAILABLE (error code: %i).", errorCode);
//                    break;
//                case GLFW_NO_WINDOW_CONTEXT:
//                    errorLoggingSystem.Log(UtilityBox::Logger::LogSeverity::ERROR, "GLFW error occurred: GLFW_NO_WINDOW_CONTEXT (error code: %i).", errorCode);
//                    break;
//                default:
//                    break;
//            }
//
//            errorLoggingSystem.Log(UtilityBox::Logger::LogSeverity::ERROR, "Provided error description: %s.", errorDescription);
//        });
    }

    void OpenGLWindow::OpenGLWindowData::SetupGLFWCallbacks() {
//        // Window close callback
//        glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
//            ServiceLocator::GetEventHub()->Dispatch(new Events::WindowCloseEvent());
//        });
//
//        // Window resize callback.
//        glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int newWidth, int newHeight) {
//            ServiceLocator::GetEventHub()->Dispatch(new Events::WindowResizeEvent(newWidth, newHeight));
//        });
//
//        // Window minimize callback.
//        glfwSetWindowIconifyCallback(_window, [](GLFWwindow* window, int minimized){
//            ServiceLocator::GetEventHub()->Dispatch(new Events::WindowMinimizedEvent((bool)minimized));
//        });
//
//        // Mouse button callback.
//        glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int mouseButton, int buttonAction, int /* MODS UNUSED */) {
//            if (buttonAction == GLFW_PRESS) {
//                ServiceLocator::GetEventHub()->Dispatch(new Events::MouseButtonPressedEvent(mouseButton));
//            }
//            else if (buttonAction == GLFW_RELEASE) {
//                ServiceLocator::GetEventHub()->Dispatch(new Events::MouseButtonReleasedEvent(mouseButton));
//            }
//        });
//
//        // Mouse scroll wheel callback.
//        glfwSetScrollCallback(_window, [](GLFWwindow* /* WINDOW UNUSED */, double xOffset, double yOffset) {
//            ServiceLocator::GetEventHub()->Dispatch(new Events::MouseScrolledEvent(xOffset, yOffset));
//        });
//
//        // Key callback.
//        glfwSetKeyCallback(_window, [](GLFWwindow* /* WINDOW UNUSED */, int keyCode, int /* SCANCODE UNUSED */, int keyAction, int /* MODS UNUSED */) {
//            if (keyAction == GLFW_PRESS) {
//                ServiceLocator::GetEventHub()->Dispatch(new Events::KeyPressEvent(keyCode));
//            }
//            else if (keyAction == GLFW_RELEASE) {
//                ServiceLocator::GetEventHub()->Dispatch(new Events::KeyReleaseEvent(keyCode));
//            }
//        });
    }

    void OpenGLWindow::OpenGLWindowData::OnUpdate() {
        glfwPollEvents();
        _context->SwapBuffers();

        _context->ClearBuffers();
    }


    //------------------------------------------------------------------------------------------------------------------
    // WINDOW
    //------------------------------------------------------------------------------------------------------------------
    OpenGLWindow::OpenGLWindow(std::string windowName, int width, int height) : _data(new OpenGLWindowData(std::move(windowName), width, height)) {
        // Nothing to do here.
    }

    OpenGLWindow::~OpenGLWindow() {
        delete _data;
    }

    int OpenGLWindow::GetWidth() const {
        return _data->GetWidth();
    }

    int OpenGLWindow::GetHeight() const {
        return _data->GetHeight();
    }

    void* OpenGLWindow::GetNativeWindow() const {
        return _data->GetNativeWindow();
    }

    void OpenGLWindow::OnUpdate() {
        _data->OnUpdate();
    }
}
