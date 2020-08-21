
#include <glad/glad.h>
#include <platform/opengl/opengl_rendering_context.h>
#include <utilitybox/logger/logging_interface.h>

namespace Spark::Graphics {
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
}
