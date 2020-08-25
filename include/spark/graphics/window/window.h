
#ifndef SPARK_WINDOW_H
#define SPARK_WINDOW_H

#include <spark/core/core.h>

namespace Spark {
    namespace Graphics {

        class IWindow {
            public:
                static IWindow* Create();
                virtual ~IWindow() = default;

                virtual void OnUpdate() = 0;

                _NODISCARD_ virtual int GetWidth() const = 0;
                _NODISCARD_ virtual int GetHeight() const = 0;
                _NODISCARD_ virtual void* GetNativeWindow() const = 0;
        };

    }
};

#endif // SPARK_WINDOW_H
