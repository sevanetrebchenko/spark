
#ifndef SPARK_IMGUI_OVERHEAD_H
#define SPARK_IMGUI_OVERHEAD_H

#include <graphics/renderer/opengl_imgui_overhead.h>
#include <GLFW/glfw3.h> // GLFWwindow

namespace Spark {
    namespace Platform {
        namespace OpenGL {

            class OpenGLImGuiOverhead : public Graphics::Renderer::ImGuiOverhead {
                public:
                    OpenGLImGuiOverhead(GLFWwindow* window, const char* glslVersion);
                    ~OpenGLImGuiOverhead() override;

                    void StartFrame() override;
                    void RenderFrame() override;
                    void EndFrame() override;

                private:
                    class ImGuiOverheadData;
                    ImGuiOverheadData* _data;
            };

        } // namespace OpenGL
    } // namespace Platform
} // namespace Spark

#endif // SPARK_IMGUI_OVERHEAD_H
