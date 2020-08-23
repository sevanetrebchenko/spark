
#include <glad/glad.h>                                    // GLenum
#include <platform/opengl/renderer/opengl_renderer_api.h> // OpenGLRendererAPI

namespace Spark::Graphics {
    void DebugMessageCallbackFn(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
        //void* pointer = const_cast<void*>(userParam);
    }

    class OpenGLRendererAPI::OpenGLRendererAPIData {
        public:
            OpenGLRendererAPIData();

        private:
    };

    OpenGLRendererAPI::OpenGLRendererAPIData::OpenGLRendererAPIData() {
#ifdef SP_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(DebugMessageCallbackFn, nullptr); // TODO: supply logging system.

        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
    }


    OpenGLRendererAPI::OpenGLRendererAPI() {

    }

    void OpenGLRendererAPI::SetViewport(unsigned x, unsigned y, unsigned width, unsigned height) {
        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::ClearBuffers() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

}
