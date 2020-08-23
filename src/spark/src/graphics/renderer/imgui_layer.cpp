
#include <platform/opengl/renderer/opengl_imgui_layer.h> // OpenGLImGuiLayer
#include <spark/graphics/renderer/imgui_layer.h>         // ImGuiLayer
#include <spark/graphics/renderer/renderer_api.h>        // RenderingAPI

namespace Spark::Graphics {

    ImGuiLayer *ImGuiLayer::Create(void *window) {
        switch(RendererAPI::GetAPI()) {
            case RendererAPI::API::NONE:
                return nullptr;
            case RendererAPI::API::OPENGL:
                return new OpenGLImGuiLayer(static_cast<GLFWwindow*>(window));
        }

        return nullptr;
    }
}
