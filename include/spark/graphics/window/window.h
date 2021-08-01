
#ifndef SPARK_WINDOW_H
#define SPARK_WINDOW_H

#include <spark/core/rename.h>

namespace Spark {
    namespace Graphics {

        class IWindow {
            public:
                static IWindow* Create();
                virtual ~IWindow() = default;

                virtual void OnUpdate() = 0;

                NODISCARD virtual int GetWidth() const = 0;
                NODISCARD virtual int GetHeight() const = 0;
                NODISCARD virtual void* GetNativeWindow() const = 0;
        };

    }
};

#endif // SPARK_WINDOW_H
