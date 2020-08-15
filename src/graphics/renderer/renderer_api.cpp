
#include <graphics/renderer/renderer_api.h>
#include <platform/opengl/opengl_renderer_api.h>

namespace Spark::Graphics {

    RendererAPI::API RendererAPI::_api = RendererAPI::API::OPENGL;

    RendererAPI *RendererAPI::Create() {
        switch (_api) {
            case RendererAPI::API::NONE:
                return nullptr;
            case RendererAPI::API::OPENGL:
                return new OpenGLRendererAPI();
        }

        return nullptr;
    }

    RendererAPI::API RendererAPI::GetAPI() {
        return RendererAPI::API::OPENGL;
    }
}
