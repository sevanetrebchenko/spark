
#include <platform/opengl/opengl_window.h> // OpenGLWindow
#include <GLFW/glfw3.h>

namespace Spark::Platform::OpenGL {

    OpenGLWindow::OpenGLWindow() : Graphics::Renderer::Window("OpenGL", 640, 480) {
        // Nothing to do here.
    }

    OpenGLWindow::~OpenGLWindow() {
        glfwDestroyWindow(static_cast<GLFWwindow*>(_window));
    }

    void OpenGLWindow::Initialize() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function Initialize for OpenGL window.");

        // Create window.
        _window = glfwCreateWindow(_windowWidth, _windowHeight, _windowName.c_str(), nullptr, nullptr);
        if (!_window) {
            message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::SEVERE);
            message.Supply("Exception thrown: GLFW window creation failed.");
            _loggingSystem.Log(message);

            throw std::runtime_error("GLFW window creation failed.");
        }

        message.Supply("Window was successfully created.");
        _loggingSystem.Log(message);
    }
}
