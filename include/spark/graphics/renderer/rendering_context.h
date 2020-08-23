
#ifndef SPARK_RENDERING_CONTEXT_H
#define SPARK_RENDERING_CONTEXT_H

#include <spark/utilitybox/tools/global_defines.h> // _NODISCARD_

namespace Spark {
    namespace Graphics {

        class RenderingContext {
            public:
                _NODISCARD_ static RenderingContext* Create(void* window);
                virtual ~RenderingContext() = default;

                virtual void SwapBuffers() = 0;
        };

    }
}

#endif // SPARK_RENDERING_CONTEXT_H
