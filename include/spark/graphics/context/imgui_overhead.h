
#ifndef SPARK_OPENGL_IMGUI_H
#define SPARK_OPENGL_IMGUI_H

namespace Spark {
    namespace Graphics {

        class ImGuiOverhead {
            public:
                static ImGuiOverhead* Create(void* window);

                virtual ~ImGuiOverhead() = default;
                virtual void BeginFrame() = 0;
                virtual void EndFrame() = 0;
        };

    }
}

#endif // SPARK_OPENGL_IMGUI_H
