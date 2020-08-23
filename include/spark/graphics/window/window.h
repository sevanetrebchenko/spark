
#ifndef SPARK_WINDOW_H
#define SPARK_WINDOW_H

#include <spark/utilitybox/tools/global_defines.h> // _NODISCARD_

namespace Spark {
    namespace Graphics {

        class Window {
            public:
                static Window* Create();
                virtual ~Window() = default;

                virtual void OnUpdate() = 0;

                _NODISCARD_ virtual int GetWidth() const = 0;
                _NODISCARD_ virtual int GetHeight() const = 0;
                _NODISCARD_ virtual void* GetNativeWindow() const = 0;
        };

    }
};

#endif // SPARK_WINDOW_H
