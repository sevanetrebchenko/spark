
#include <spark/graphics/window/window.h>         // Window
#include <spark/graphics/renderer/renderer_api.h> // RendererAPI
#include <platform/opengl/window/opengl_window.h> // OpenGLWindow

namespace Spark::Graphics {

    IWindow* IWindow::Create() {
        switch (IRenderingAPI::GetAPI()) {
            case IRenderingAPI::API::NONE:
                return nullptr;
            case IRenderingAPI::API::OPENGL:
                return new Graphics::OpenGLWindow("Spark Engine - OpenGL", 1080, 720);
        }

        return nullptr;
    }

}
