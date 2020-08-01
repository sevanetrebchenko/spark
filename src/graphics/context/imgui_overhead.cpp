
#include <graphics/context/imgui_overhead.h> // ImGuiOverhead

namespace Spark::Graphics::Context {
    // Constructor.
    ImGuiOverhead::ImGuiOverhead(RenderingAPI API) : _api(API) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entered base ImGui overhead constructor.");
        message.Supply("Selected API: %s.", APIToString(_api));
        _loggingSystem.Log(message);
    }

    // Destructor.
    ImGuiOverhead::~ImGuiOverhead() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entered base ImGuiOverhead destructor.");
        _loggingSystem.Log(message);
    }
}
