
#include <graphics/context/rendering_context.h>
#include <platform/opengl/opengl_rendering_context.h>

namespace Spark::Graphics {
    RenderingContext* RenderingContext::Create(void *window) {
        // Only supports OpenGL for now. TODO: conditional switch based on API / native rendering
        return new OpenGLRenderingContext(static_cast<GLFWwindow*>(window));
    }
}
