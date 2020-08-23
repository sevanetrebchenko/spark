
#ifndef SPARK_OPENGL_RENDERER_API_H
#define SPARK_OPENGL_RENDERER_API_H

#include <glm/glm.hpp>                            // glm::vec4
#include <spark/graphics/renderer/renderer_api.h> // RendererAPI

namespace Spark {
    namespace Graphics {

        class OpenGLRendererAPI : public RendererAPI {
            public:
                OpenGLRendererAPI();

                void SetViewport(unsigned x, unsigned y, unsigned width, unsigned height) override;
                void SetClearColor(const glm::vec4& color) override;
                void ClearBuffers() override;

            private:
                class OpenGLRendererAPIData;
                OpenGLRendererAPIData* _data;
        };

    }
}

#endif //SPARK_OPENGL_RENDERER_API_H
