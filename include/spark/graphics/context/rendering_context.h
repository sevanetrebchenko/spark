
#ifndef SPARK_RENDERING_CONTEXT_H
#define SPARK_RENDERING_CONTEXT_H

#include <glad/glad.h>                       // Glad
#include <GLFW/glfw3.h>                      // GLFWwindow
#include <utilitybox/tools/global_defines.h> // _NODISCARD_
#include <graphics/context/window.h>         // Window
#include <graphics/context/imgui_overhead.h> // ImGuiOverhead
#include <graphics/context/rendering_api.h>  // RenderingAPI

namespace Spark {
    namespace Graphics {
        namespace Context {

            class RenderingContext {
                public:
                    /**
                     * Construct a rendering context with a given API. Constructor initializes everything related to the
                     * window and ImGui overhead
                     * @param API - Platform-specific selection to indicate which API to use while initializing the rendering
                     *              context.
                     */
                    explicit RenderingContext(RenderingAPI API);

                    /**
                     * Cleans up the window and ImGui overhead related to this rendering context.
                     */
                    virtual ~RenderingContext();

                    /**
                     * Get the underlying context window object.
                     * @return Pointer to window.
                     */
                    _NODISCARD_ Window* GetWindow() const;

                    /**
                     * Get the underlying context ImGui overhead object.
                     * @return Pointer to ImGui overhead.
                     */
                    _NODISCARD_ ImGuiOverhead* GetImGui() const;

                protected:
                    RenderingAPI _api;             // API to use for the rendering context.
                    Window* _window;               // Window.
                    ImGuiOverhead* _imGuiOverhead; // ImGui overhead.
                    UtilityBox::Logger::LoggingSystem _loggingSystem { std::string("Rendering Context - ") + std::string(APIToString(_api)) };
            };

        } // namespace Context
    } // namespace Graphics
} // namespace Spark

#endif // SPARK_RENDERING_CONTEXT_H
