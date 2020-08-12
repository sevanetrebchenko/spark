
#ifndef SPARK_OPENGL_RENDERING_CONTEXT_H
#define SPARK_OPENGL_RENDERING_CONTEXT_H

#include <graphics/context/rendering_context.h>
#include <spark_pch.h>
#include "opengl_window.h"
#include <graphics/context/imgui_overhead.h>

namespace Spark {
    namespace Platform {
        namespace OpenGL {

            class OpenGLRenderingContext : public Graphics::Context::RenderingContext {
                public:
                    OpenGLRenderingContext();
                    ~OpenGLRenderingContext() override;

                private:
                    class OpenGLRenderingContextData;
                    OpenGLRenderingContextData* _data;
            };

        } // namespace OpenGL
    } // namespace Platform
} // namespace Spark

#endif // SPARK_OPENGL_RENDERING_CONTEXT_H
