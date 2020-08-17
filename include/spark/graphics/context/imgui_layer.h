
#ifndef SPARK_IMGUI_LAYER_H
#define SPARK_IMGUI_LAYER_H

#include <core/layer.h>

namespace Spark {
    namespace Graphics {

        class ImGuiLayer : public Layer {
            public:
                static ImGuiLayer* Create(void* window);
                virtual void BeginFrame() = 0;
                virtual void EndFrame() = 0;
        };

    }
}

#endif // SPARK_IMGUI_LAYER_H
