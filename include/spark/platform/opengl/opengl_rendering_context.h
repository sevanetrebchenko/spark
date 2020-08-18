#ifndef SPARK_OPENGL_RENDERING_CONTEXT_H
#define SPARK_OPENGL_RENDERING_CONTEXT_H

#include <graphics/context/rendering_context.h>
#include <GLFW/glfw3.h>

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

    } // namespace Graphics
} // namespace Spark

#endif // SPARK_OPENGL_RENDERING_CONTEXT_H
