
#include <graphics/renderer/window.h> // Window

namespace Spark::Graphics::Renderer {
    // Construct a base window to use across all platforms.
    Window::Window(std::string windowName, int width, int height) : _window(nullptr),
                                                                    _windowName(std::move(windowName)),
                                                                    _windowWidth(width),
                                                                    _windowHeight(height) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering Window constructor.");
        message.Supply("Provided window name: '%s'.", _windowName.c_str());
        message.Supply("Window width: %i.", _windowWidth);
        message.Supply("Window height: %i.", _windowHeight);
        _loggingSystem.Log(message);
    }

    Window::~Window() {

    }

    // Get the pointer to the underlying window object.
    void* Window::GetNativeWindow() const {
        if (!_window) {
            UtilityBox::Logger::LogMessage message { UtilityBox::Logger::LogMessageSeverity::WARNING };
            message.Supply("Attempting to get a native window that hasn't been initialized yet.");
            _loggingSystem.Log(message);
        }

        return _window;
    }
}
