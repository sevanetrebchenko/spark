
#ifndef SPARK_WINDOW_H
#define SPARK_WINDOW_H

#include <utilitybox/tools/global_defines.h>  // _NODISCARD_
#include <utilitybox/logger/logging_system.h> // LoggingSystem
#include <spark_pch.h>                        // std::string

namespace Spark {
    namespace Graphics {
        namespace Context {

            class Window {
                public:
                    /**
                     * Initialize a window based on the platform-specific rendering context being used.
                     * @param windowName - Name for the window to have upon creation.
                     * @param width      - Desired width of the window.
                     * @param height     - Desired height of the window.
                     */
                    Window(std::string windowName, int width, int height);

                    /**
                     * Destructor.
                     */
                    virtual ~Window();

                    /**
                     * Get the pointer to the underlying window object.
                     * @return Pointer to the base window.
                     */
                    _NODISCARD_ void* GetNativeWindow() const;

                protected:
                    std::string _windowName; // Name of the window.
                    int _windowWidth;        // Window width.
                    int _windowHeight;       // Window height
                    void* _window;           // Window object.

                    UtilityBox::Logger::LoggingSystem _loggingSystem { std::string("Window - ") + _windowName }; // Window logging system.
            };

        } // namespace Context
    } // namespace Graphics
} // namespace Spark

#endif // SPARK_WINDOW_H
