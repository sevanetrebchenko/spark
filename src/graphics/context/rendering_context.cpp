
#include <graphics/context/rendering_context.h> // RenderingContext

namespace Spark::Graphics::Context {
    // Create a rendering context with a given API. Context is uninitialized until function Initialize() is called.
    RenderingContext::RenderingContext(RenderingAPI api) : _api(api), _window(nullptr), _imGuiOverhead(nullptr) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entered base RenderingContext constructor.");
        message.Supply("Selected API: %s.", APIToString(_api));
        _loggingSystem.Log(message);
    }

    // Cleans up resources associated with this rendering context. Closes/terminates the window.
    RenderingContext::~RenderingContext() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entered base RenderingContext destructor.");
        _loggingSystem.Log(message);
    }

    // Get the underlying context window object.
    Window* RenderingContext::GetWindow() const {
        return _window;
    }

    // Get the underlying context ImGui overhead object.
    ImGuiOverhead* RenderingContext::GetImGui() const {
        return _imGuiOverhead;
    }
}
