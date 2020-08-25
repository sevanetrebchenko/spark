
#ifndef SPARK_IMGUI_LAYER_H
#define SPARK_IMGUI_LAYER_H

#include <spark/core/layer.h>                      // Layer
#include <spark/utilitybox/tools/global_defines.h> // _NODISCARD_

namespace Spark {
    namespace Graphics {

        class IImGuiLayer : public Layer {
            public:
                _NODISCARD_ static IImGuiLayer* Create(void* window);
                virtual void BeginFrame() = 0;
                virtual void EndFrame() = 0;
        };

    }
}

#endif // SPARK_IMGUI_LAYER_H
