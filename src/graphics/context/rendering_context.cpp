
#include <graphics/context/rendering_context.h>
#include <graphics/renderer/renderer_api.h>
#include <platform/opengl/opengl_rendering_context.h>

namespace Spark::Graphics {
    RenderingContext* RenderingContext::Create(void *window) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::NONE:
                return nullptr;
            case RendererAPI::API::OPENGL:
                return new OpenGLRenderingContext(static_cast<GLFWwindow*>(window));
        }

        return nullptr;
    }
}
