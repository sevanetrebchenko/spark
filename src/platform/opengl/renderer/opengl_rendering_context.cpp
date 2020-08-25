
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <spark/core/core.h>
#include <platform/opengl/renderer/opengl_rendering_context.h> // OpenGLRenderingContext
#include <spark/utilitybox/logger/logging_interface.h>         // ILoggable

namespace Spark::Graphics {
    void DebugMessageCallbackFn(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    }

    //------------------------------------------------------------------------------------------------------------------
    // OPENGL RENDERING CONTEXT DATA
    //------------------------------------------------------------------------------------------------------------------
    class OpenGLRenderingContext::OpenGLRenderingContextData : public UtilityBox::Logger::ILoggable {
        public:
            explicit OpenGLRenderingContextData(GLFWwindow* window);
            ~OpenGLRenderingContextData();
            void SwapBuffers();

        private:
            GLFWwindow* _window;
    };

    OpenGLRenderingContext::OpenGLRenderingContextData::OpenGLRenderingContextData(GLFWwindow* window) : _window(window), UtilityBox::Logger::ILoggable("OpenGL Rendering Context") {
        // TODO: assert that window is not null.
        // Set current OpenGL context to this window.
        glfwMakeContextCurrent(_window);

        int initializationCode = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        // Failed to initialize Glad.
        if (!initializationCode) {
            throw std::runtime_error("Glad initialization failed.");
        }

        glfwSwapInterval(1);

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

    OpenGLRenderingContext::OpenGLRenderingContextData::~OpenGLRenderingContextData() {
        glfwTerminate();
    }

    void OpenGLRenderingContext::OpenGLRenderingContextData::SwapBuffers() {
        glfwSwapBuffers(_window);
    }

    //------------------------------------------------------------------------------------------------------------------
    // OPENGL RENDERING CONTEXT
    //------------------------------------------------------------------------------------------------------------------
    OpenGLRenderingContext::OpenGLRenderingContext(GLFWwindow* window) : _data(new OpenGLRenderingContextData(window)) {
        // Nothing to do here.
    }

    OpenGLRenderingContext::~OpenGLRenderingContext() {
        delete _data;
    }

    void OpenGLRenderingContext::SwapBuffers() {
        _data->SwapBuffers();
    }

    void OpenGLRenderingContext::SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
        glViewport(x, y, width, height);
    }

    void OpenGLRenderingContext::SetClearColor(const glm::vec4 &color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRenderingContext::ClearBuffers() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

}
