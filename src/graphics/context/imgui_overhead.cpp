
#include <graphics/context/imgui_overhead.h>
#include <platform/opengl/opengl_imgui_overhead.h>
#include <graphics/renderer/renderer_api.h>

namespace Spark::Graphics {

    ImGuiOverhead* ImGuiOverhead::Create(void* window) {
        switch(RendererAPI::GetAPI()) {
            case RendererAPI::API::NONE:
                return nullptr;
            case RendererAPI::API::OPENGL:
                return new OpenGLImGuiOverhead(static_cast<GLFWwindow*>(window));
        }

        return nullptr;
    }

}
