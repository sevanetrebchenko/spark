
#include <graphics/context/rendering_api.h>

namespace Spark::Graphics::Context {
    // Convert rendering API selection to a human-readable string.
    const char* APIToString(RenderingAPI API) {
        switch (API) {
            case RenderingAPI::OPENGL:
                return "OpenGL";
        }
    }
}

