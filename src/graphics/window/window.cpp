
#include <graphics/window/window.h>
#include <graphics/renderer/renderer_api.h>
#include <platform/opengl/opengl_window.h>

namespace Spark::Graphics {

    Window* Window::Create() {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::NONE:
                return nullptr;
            case RendererAPI::API::OPENGL:
                return new Graphics::OpenGLWindow("Spark Engine - OpenGL", 1080, 720);
        }

        return nullptr;
    }

}
