
#include <spark/graphics/renderer/renderer_api.h>         // RendererAPI

namespace Spark::Graphics {
    IRenderingAPI::API IRenderingAPI::_api = IRenderingAPI::API::OPENGL;

    IRenderingAPI::API IRenderingAPI::GetAPI() {
        return IRenderingAPI::API::OPENGL;
    }
}
