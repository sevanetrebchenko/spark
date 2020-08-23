
#ifndef SPARK_RENDERER_API_H
#define SPARK_RENDERER_API_H

#include <glm/glm.hpp>                             // glm::vec4
#include <spark/utilitybox/tools/global_defines.h> // _NODISCARD_

namespace Spark {
    namespace Graphics {

        class RendererAPI {
            public:
                enum class API {
                    NONE = 0,
                    OPENGL = 1
                };

                _NODISCARD_ static RendererAPI* Create();
                _NODISCARD_ static API GetAPI();

                virtual ~RendererAPI() = default;
                virtual void SetViewport(unsigned x, unsigned y, unsigned width, unsigned height) = 0;
                virtual void SetClearColor(const glm::vec4& color) = 0;
                virtual void ClearBuffers() = 0;

            private:
                static API _api;
        };

    }
}

#endif // SPARK_RENDERER_API_H
