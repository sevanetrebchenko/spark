#ifndef SPARK_OPENGL_WINDOW_H
#define SPARK_OPENGL_WINDOW_H

#include <spark/core/rename.h>
#include <spark/graphics/window/window.h>    // Window

namespace Spark {
    namespace Graphics {

        class OpenGLWindow : public IWindow {
            public:
                OpenGLWindow(std::string windowName, int width, int height);
                ~OpenGLWindow() override;

                void OnUpdate() override;

                NODISCARD int GetWidth() const override;
                NODISCARD int GetHeight() const override;
                NODISCARD void* GetNativeWindow() const override;

            private:
                class OpenGLWindowData;
                OpenGLWindowData* _data;
        };

    }
}

#endif // SPARK_OPENGL_WINDOW_H
