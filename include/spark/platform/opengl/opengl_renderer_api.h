
#ifndef SPARK_OPENGL_RENDERER_API_H
#define SPARK_OPENGL_RENDERER_API_H

#include <graphics/renderer/renderer_api.h>
#include <glad/glad.h>

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
