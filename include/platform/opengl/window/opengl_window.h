#ifndef SPARK_OPENGL_WINDOW_H
#define SPARK_OPENGL_WINDOW_H

#include <spark_pch.h>                             // std::string
#include <spark/utilitybox/tools/global_defines.h> // _NODISCARD_
#include <spark/graphics/window/window.h>          // Window

namespace Spark {
    namespace Graphics {

        class OpenGLWindow : public IWindow {
            public:
                OpenGLWindow(std::string windowName, int width, int height);
                ~OpenGLWindow() override;

                void OnUpdate() override;

                _NODISCARD_ int GetWidth() const override;
                _NODISCARD_ int GetHeight() const override;
                _NODISCARD_ void* GetNativeWindow() const override;

            private:
                class OpenGLWindowData;
                OpenGLWindowData* _data;
        };

    }
}

#endif // SPARK_OPENGL_WINDOW_H
