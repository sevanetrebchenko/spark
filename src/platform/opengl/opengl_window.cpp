
#include "../platform/opengl/opengl_window.h"

namespace Spark::Platform::OpenGL {

    // Creates a GLFW window. Function throws error on creation failure.
    OpenGLWindow::OpenGLWindow() : Graphics::Context::Window("OpenGL", 1920, 1080) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering constructor for OpenGL window.");

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

    // Destroys created GLFW window.
    OpenGLWindow::~OpenGLWindow() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering destructor for OpenGL window.");

        glfwDestroyWindow(static_cast<GLFWwindow*>(_window));
        message.Supply("GLFW window successfully destroyed.");

        _loggingSystem.Log(message);
    }
}
