

#include <spark/graphics/window/window.h>         // Window
#include <spark/graphics/renderer/renderer_api.h> // RendererAPI
#include <platform/opengl/window/opengl_window.h> // OpenGLWindow

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
