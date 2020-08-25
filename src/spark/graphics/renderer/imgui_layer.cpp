
#include <platform/opengl/renderer/opengl_imgui_layer.h> // OpenGLImGuiLayer
#include <spark/graphics/renderer/imgui_layer.h>         // ImGuiLayer
#include <spark/graphics/renderer/renderer_api.h>        // RenderingAPI

namespace Spark::Graphics {

    IImGuiLayer *IImGuiLayer::Create(void *window) {
        switch(IRenderingAPI::GetAPI()) {
            case IRenderingAPI::API::NONE:
                return nullptr;
            case IRenderingAPI::API::OPENGL:
                return new OpenGLImGuiLayer(static_cast<GLFWwindow*>(window));
        }

        return nullptr;
    }
}
