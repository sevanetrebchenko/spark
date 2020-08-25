
#ifndef SPARK_RENDERING_CONTEXT_H
#define SPARK_RENDERING_CONTEXT_H

#include <utilitybox/tools/global_defines.h> // _NODISCARD_
#include <spark/graphics/renderer/renderer_api.h>  // RenderingAPI

namespace Spark {
    namespace Graphics {

        class IRenderingContext : public IRenderingAPI {
            public:
                _NODISCARD_ static IRenderingContext* Create(void* window);
                virtual ~IRenderingContext() = default;

                virtual void SwapBuffers() = 0;
        };

    }
}

#endif // SPARK_RENDERING_CONTEXT_H
