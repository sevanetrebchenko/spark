#ifndef SPARK_OPENGL_RENDERING_CONTEXT_H
#define SPARK_OPENGL_RENDERING_CONTEXT_H

#include <GLFW/glfw3.h>                                // GLFWwindow
#include <spark/graphics/renderer/rendering_context.h> // RenderingContext

namespace Spark {
    namespace Graphics {

        class OpenGLRenderingContext : public RenderingContext {
            public:
                explicit OpenGLRenderingContext(GLFWwindow* window);
                ~OpenGLRenderingContext() override;

                void SwapBuffers() override;

            private:
                class OpenGLRenderingContextData;
                OpenGLRenderingContextData* _data;
        };

    }
}

#endif // SPARK_OPENGL_RENDERING_CONTEXT_H
