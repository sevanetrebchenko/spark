
#ifndef SPARK_OPENGL_WINDOW_H
#define SPARK_OPENGL_WINDOW_H

#include <graphics/renderer/window.h> // Window
#include <GLFW/glfw3.h>

namespace Spark {
    namespace Platform {
        namespace OpenGL {

            class OpenGLWindow final : public Graphics::Renderer::Window {
                public:
                    OpenGLWindow();

                    ~OpenGLWindow() override;
                    void Initialize() override;

                private:
            };

        } // namespace OpenGL
    } // namespace Platform
} // namespace Spark

#endif //SPARK_OPENGL_WINDOW_H
