
#include <graphics/context/imgui_layer.h>
#include <graphics/renderer/renderer_api.h>
#include <platform/opengl/opengl_imgui_layer.h>

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
