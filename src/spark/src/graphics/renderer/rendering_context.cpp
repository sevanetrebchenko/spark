
#include <GLFW/glfw3.h>                                        // GLFWwindow
#include <spark/graphics/renderer/rendering_context.h>         // RenderingContext
#include <spark/graphics/renderer/renderer_api.h>              // RendererAPI
#include <platform/opengl/renderer/opengl_rendering_context.h> // OpenGLRenderingContext

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
