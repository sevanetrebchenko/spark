
#ifndef SPARK_RENDERER_API_H
#define SPARK_RENDERER_API_H

#include <spark/core/rename.h>
#include <glm/glm.hpp>       // glm::vec4

namespace Spark {
    namespace Graphics {

        class IRenderingAPI {
            public:
                enum class API {
                    NONE = 0,
                    OPENGL = 1
                };

                NODISCARD static API GetAPI();
                virtual void SetViewport(unsigned x, unsigned y, unsigned width, unsigned height) = 0;
                virtual void SetClearColor(const glm::vec4& color) = 0;
                virtual void ClearBuffers() = 0;

            private:
                static API _api;
        };

    }
}

#endif // SPARK_RENDERER_API_H
