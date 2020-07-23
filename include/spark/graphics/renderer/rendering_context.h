
#ifndef SPARK_RENDERING_CONTEXT_H
#define SPARK_RENDERING_CONTEXT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>                      // GLFWwindow

#include <utilitybox/tools/global_defines.h> // _NODISCARD_
#include <graphics/renderer/window.h>
#include <graphics/renderer/opengl_imgui_overhead.h>

namespace Spark {
    namespace Graphics {
        namespace Renderer {

            class RenderingContext {
                public:
                    enum class RenderingAPI {
                        OPENGL
                        // TODO: VULCAN, DX
                    };

                    explicit RenderingContext(RenderingAPI API);
                    virtual ~RenderingContext();

                    _NODISCARD_ const RenderingAPI& GetRenderingAPI() const;

                    virtual void Initialize() = 0;
                    _NODISCARD_ virtual const Window* GetWindow() const = 0;
                    _NODISCARD_ virtual ImGuiOverhead* GetImGui() const = 0;

                protected:
                    const RenderingAPI _api; // API to use.
            };

        } // namespace Base
    } // namespace Platform
} // namespace Spark

#endif // SPARK_RENDERING_CONTEXT_H
