
#ifndef SPARK_WINDOW_H
#define SPARK_WINDOW_H

#include <utilitybox/tools/global_defines.h>  // _NODISCARD_
#include <utilitybox/logger/logging_system.h> // LoggingSystem
#include <spark_pch.h>                        // std::string

namespace Spark {
    namespace Graphics {
        namespace Renderer {

            class Window {
                public:
                    /**
                     * Construct a base window to use across all platforms.
                     * @param windowName - Name for the window to have upon creation.
                     * @param width      - Desired width of the window.
                     * @param height     - Desired height of the window.
                     */
                    Window(std::string windowName, int width, int height);

                    /**
                     * Destructor.
                     */
                    virtual ~Window() = 0;

                    /**
                     * Initialize a window to the platform it is running on.
                     */
                    virtual void Initialize() = 0;

                    /**
                     * Get the pointer to the underlying window object.
                     * @return
                     */
                    _NODISCARD_ void* GetNativeWindow() const;

                protected:
                    std::string _windowName; // Name of the window.
                    int _windowWidth;        // Window width.
                    int _windowHeight;       // Window height
                    void* _window;           // Window.

                    UtilityBox::Logger::LoggingSystem _loggingSystem { std::string("Window: ") + _windowName };
            };

        } // namespace Renderer
    } // namespace Graphics
} // namespace Spark

#endif // SPARK_WINDOW_H
