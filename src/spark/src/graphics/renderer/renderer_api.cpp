
#include <spark/graphics/renderer/renderer_api.h>         // RendererAPI
#include <platform/opengl/renderer/opengl_renderer_api.h> // OpenGLRendererAPI

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
