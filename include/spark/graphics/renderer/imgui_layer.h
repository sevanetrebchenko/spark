
#ifndef SPARK_IMGUI_LAYER_H
#define SPARK_IMGUI_LAYER_H

#include <spark/core/rename.h>
#include <spark/core/layer.h> // Layer

namespace Spark {
    namespace Graphics {

        class IImGuiLayer : public Layer {
            public:
                NODISCARD static IImGuiLayer* Create(void* window);
                virtual void BeginFrame() = 0;
                virtual void EndFrame() = 0;
        };

    }
}

#endif // SPARK_IMGUI_LAYER_H
