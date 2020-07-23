
#ifndef SPARK_OPENGL_IMGUI_OVERHEAD_H
#define SPARK_OPENGL_IMGUI_OVERHEAD_H

namespace Spark {
    namespace Graphics {
        namespace Renderer {

            class ImGuiOverhead {
                public:
                    ImGuiOverhead();
                    virtual ~ImGuiOverhead() = 0;

                    virtual void StartFrame() = 0;
                    virtual void RenderFrame() = 0;
                    virtual void EndFrame() = 0;
            };

        }
    }
}

#endif //SPARK_OPENGL_IMGUI_OVERHEAD_H
