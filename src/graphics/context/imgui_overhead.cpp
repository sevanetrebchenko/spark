
#include <graphics/context/imgui_overhead.h>
#include <platform/opengl/opengl_imgui_overhead.h>


namespace Spark::Graphics {
    ImGuiOverhead* ImGuiOverhead::Create(void* window) {
        return new OpenGLImGuiOverhead(static_cast<GLFWwindow*>(window));
    }

}
