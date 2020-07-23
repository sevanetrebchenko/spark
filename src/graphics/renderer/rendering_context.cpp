
#include <graphics/renderer/rendering_context.h> // RenderingContext

namespace Spark::Graphics::Renderer {
    // Create a rendering context with a given API. Context is uninitialized until function Initialize() is called.
    RenderingContext::RenderingContext(RenderingContext::RenderingAPI api) : _api(api) {
        // Nothing to do here.
    }

    // Cleans up resources associated with this rendering context. Closes/terminates the window.
    RenderingContext::~RenderingContext() {
    }

    // Get the rendering API associated with this rendering context.
    const RenderingContext::RenderingAPI &RenderingContext::GetRenderingAPI() const {
        return _api;
    }
}
