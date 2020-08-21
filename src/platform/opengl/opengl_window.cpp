
#include <platform/opengl/opengl_window.h>
#include <platform/opengl/opengl_rendering_context.h>
#include <core/service_locator.h>
#include <events/key_events.h>
#include <events/mouse_events.h>
#include <utilitybox/logger/logging_interface.h>
#include <GLFW/glfw3.h>
#include <utilitybox/tools/utility_functions.h>

namespace Spark::Graphics {
    //------------------------------------------------------------------------------------------------------------------
    // WINDOW DATA
    //------------------------------------------------------------------------------------------------------------------
    class OpenGLWindow::OpenGLWindowData : public UtilityBox::Logger::ILoggable {
        public:
            OpenGLWindowData(std::string windowName, int width, int height);
            ~OpenGLWindowData();

            void OnUpdate();

            _NODISCARD_ int GetWidth() const;
            _NODISCARD_ int GetHeight() const;
            _NODISCARD_ GLFWwindow* GetNativeWindow() const;

        private:
            void InitializeGLFW();
            void SetupGLFWCallbacks();
            void LogErrorWrapper(const char*formattingString, int errorCode, const char *errorDescription);

            std::string _windowName;
            int _windowWidth, _windowHeight;

            GLFWwindow* _window;
            Graphics::RenderingContext* _context;
    };

    OpenGLWindow::OpenGLWindowData::OpenGLWindowData(std::string windowName, int width, int height) : _windowName(std::move(windowName)),
                                                                                                      _windowWidth(width),
                                                                                                      _windowHeight(height),
                                                                                                      UtilityBox::Logger::ILoggable("OpenGL Window") {
        // GLFW needs to be initialized before creating a GLFW window.
        InitializeGLFW();

        // TODO: Support higher version of OpenGL
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create window.
        _window = glfwCreateWindow(_windowWidth, _windowHeight, _windowName.c_str(), nullptr, nullptr);
        if (!_window) {
            throw std::runtime_error("GLFW window creation failed.");
        }

        // Create rendering context.
        _context = Graphics::RenderingContext::Create(_window);
        // TODO: assert

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

    void OpenGLWindow::OpenGLWindowData::InitializeGLFW() {
        int initializationCode = glfwInit();
        // Failed to initialize GLFW
        if (!initializationCode) {
            LogError("GLFW initialization failed with error code: %i.", initializationCode);
            throw std::runtime_error("GLFW initialization failed.");
        }
        else {
            LogDebug("GLFW successfully initialized.");
        }

        glfwSetErrorCallback([](int errorCode, const char *errorDescription) {
            static UtilityBox::Logger::LoggingSystem errorLoggingSystem("GLFW Error");

            switch (errorCode) {
                case GLFW_NO_ERROR:
                    errorLoggingSystem.Log(UtilityBox::Logger::LogMessageSeverity::SEVERE, "GLFW error occurred: GLFW_NO_ERROR (error code: %i).", errorCode);
                    break;
                case GLFW_NOT_INITIALIZED:
                    errorLoggingSystem.Log(UtilityBox::Logger::LogMessageSeverity::SEVERE, "GLFW error occurred: GLFW_NOT_INITIALIZED (error code: %i).", errorCode);
                    break;
                case GLFW_NO_CURRENT_CONTEXT:
                    errorLoggingSystem.Log(UtilityBox::Logger::LogMessageSeverity::SEVERE, "GLFW error occurred: GLFW_NO_CURRENT_CONTEXT (error code: %i).", errorCode);
                    break;
                case GLFW_INVALID_ENUM:
                    errorLoggingSystem.Log(UtilityBox::Logger::LogMessageSeverity::SEVERE, "GLFW error occurred: GLFW_INVALID_ENUM (error code: %i).", errorCode);
                    break;
                case GLFW_INVALID_VALUE:
                    errorLoggingSystem.Log(UtilityBox::Logger::LogMessageSeverity::SEVERE, "GLFW error occurred: GLFW_INVALID_VALUE (error code: %i).", errorCode);
                    break;
                case GLFW_OUT_OF_MEMORY:
                    errorLoggingSystem.Log(UtilityBox::Logger::LogMessageSeverity::SEVERE, "GLFW error occurred: GLFW_OUT_OF_MEMORY (error code: %i).", errorCode);
                    break;
                case GLFW_API_UNAVAILABLE:
                    errorLoggingSystem.Log(UtilityBox::Logger::LogMessageSeverity::SEVERE, "GLFW error occurred: GLFW_API_UNAVAILABLE (error code: %i).", errorCode);
                    break;
                case GLFW_VERSION_UNAVAILABLE:
                    errorLoggingSystem.Log(UtilityBox::Logger::LogMessageSeverity::SEVERE, "GLFW error occurred: GLFW_VERSION_UNAVAILABLE (error code: %i).", errorCode);
                    break;
                case GLFW_PLATFORM_ERROR:
                    errorLoggingSystem.Log(UtilityBox::Logger::LogMessageSeverity::SEVERE, "GLFW error occurred: GLFW_PLATFORM_ERROR (error code: %i).", errorCode);
                    break;
                case GLFW_FORMAT_UNAVAILABLE:
                    errorLoggingSystem.Log(UtilityBox::Logger::LogMessageSeverity::SEVERE, "GLFW error occurred: GLFW_FORMAT_UNAVAILABLE (error code: %i).", errorCode);
                    break;
                case GLFW_NO_WINDOW_CONTEXT:
                    errorLoggingSystem.Log(UtilityBox::Logger::LogMessageSeverity::SEVERE, "GLFW error occurred: GLFW_NO_WINDOW_CONTEXT (error code: %i).", errorCode);
                    break;
                default:
                    break;
            }

            errorLoggingSystem.Log(UtilityBox::Logger::LogMessageSeverity::SEVERE, "Provided error description: %s.", errorDescription);
        });
    }

    void OpenGLWindow::OpenGLWindowData::SetupGLFWCallbacks() {
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
    }

    void OpenGLWindow::OpenGLWindowData::OnUpdate() {
        glfwPollEvents();
        _context->SwapBuffers();
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
