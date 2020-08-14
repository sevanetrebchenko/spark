#ifndef SPARK_OPENGL_WINDOW_H
#define SPARK_OPENGL_WINDOW_H

#include <spark_pch.h>
#include <utilitybox/tools/global_defines.h>
#include <graphics/window/window.h>

namespace Spark {
    namespace Graphics {

        class OpenGLWindow : public Window {
            public:
                OpenGLWindow(std::string windowName, int width, int height);
                ~OpenGLWindow() override;

                _NODISCARD_ int GetWidth() const override;
                _NODISCARD_ int GetHeight() const override;
                _NODISCARD_ void* GetNativeWindow() const override;

            private:
                class OpenGLWindowData;
                OpenGLWindowData* _data;
        };

    }
}

#endif //SPARK_OPENGL_WINDOW_H
