
#ifndef SPARK_OPENGL_IMGUI_OVERHEAD_H
#define SPARK_OPENGL_IMGUI_OVERHEAD_H

#include <graphics/context/imgui_overhead.h>
#include <GLFW/glfw3.h>

namespace Spark {
    namespace Graphics {

        class OpenGLImGuiOverhead : public ImGuiOverhead {
            public:
                explicit OpenGLImGuiOverhead(GLFWwindow* window);
                ~OpenGLImGuiOverhead() override;

                void BeginFrame() override;
                void EndFrame() override;

            private:
                class OpenGLImGuiOverheadData;
                OpenGLImGuiOverheadData* _data;
        };

    }
}

#endif //SPARK_OPENGL_IMGUI_OVERHEAD_H
