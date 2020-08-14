
#ifndef SPARK_RENDERING_CONTEXT_H
#define SPARK_RENDERING_CONTEXT_H

namespace Spark {
    namespace Graphics {

        class RenderingContext {
            public:
                static RenderingContext* Create(void* window);
                virtual ~RenderingContext() = default;
        };

    }
}

#endif //SPARK_RENDERING_CONTEXT_H
