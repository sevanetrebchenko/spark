
#ifndef SPARK_OPENGL_RENDERING_CONTEXT_H
#define SPARK_OPENGL_RENDERING_CONTEXT_H

#include <graphics/renderer/rendering_context.h>
#include <spark_pch.h>
#include <platform/opengl/opengl_window.h>

namespace Spark {
    namespace Platform {
        namespace OpenGL {

            class OpenGLRenderingContext : public Graphics::Renderer::RenderingContext {
                public:
                    OpenGLRenderingContext();
                    ~OpenGLRenderingContext() override;

                    void Initialize() override;
                    _NODISCARD_ const Graphics::Renderer::Window* GetWindow() const override;
                    _NODISCARD_ Graphics::Renderer::ImGuiOverhead* GetImGui() const override;

                private:
                    class OpenGLRenderingContextData;
                    OpenGLRenderingContextData* _data;
            };

        } // namespace OpenGL
    } // namespace Platform
} // namespace Spark

#endif // SPARK_OPENGL_RENDERING_CONTEXT_H
