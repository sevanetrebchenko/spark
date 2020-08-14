
#include <graphics/window/window.h>
#include <platform/opengl/opengl_window.h>

namespace Spark::Graphics {
    Window* Window::Create() {
        // Only supports OpenGL for now. TODO: conditional switch based on API / native rendering
        return new Graphics::OpenGLWindow("Spark Engine - OpenGL", 1080, 720);
    }
}
