
#ifndef SPARK_OPENGL_WINDOW_H
#define SPARK_OPENGL_WINDOW_H

#include <graphics/context/window.h> // Window
#include <GLFW/glfw3.h>              // GLFWwindow

namespace Spark {
    namespace Platform {
        namespace OpenGL {

            class OpenGLWindow final : public Graphics::Context::Window {
                public:
                    /**
                     * Creates a GLFW window. Function throws error on creation failure.
                     */
                    OpenGLWindow();

                    /**
                     * Destroys created GLFW window.
                     */
                    ~OpenGLWindow() override;
            };

        } // namespace OpenGL
    } // namespace Platform
} // namespace Spark

#endif // SPARK_OPENGL_WINDOW_H
