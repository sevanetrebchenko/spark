
#include <glad/glad.h>
#include <platform/opengl/opengl_rendering_context.h>
#include <utilitybox/logger/logging_system.h>

namespace Spark::Graphics {
    //------------------------------------------------------------------------------------------------------------------
    // OPENGL RENDERING CONTEXT DATA
    //------------------------------------------------------------------------------------------------------------------
    class OpenGLRenderingContext::OpenGLRenderingContextData {
        public:
            explicit OpenGLRenderingContextData(GLFWwindow* window);
            ~OpenGLRenderingContextData();

        private:
            GLFWwindow* _window;
            UtilityBox::Logger::LoggingSystem _loggingSystem;
    };

    OpenGLRenderingContext::OpenGLRenderingContextData::OpenGLRenderingContextData(GLFWwindow* window) : _window(window) {
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


    //------------------------------------------------------------------------------------------------------------------
    // OPENGL RENDERING CONTEXT
    //------------------------------------------------------------------------------------------------------------------
    OpenGLRenderingContext::OpenGLRenderingContext(GLFWwindow* window) : _data(new OpenGLRenderingContextData(window)) {
        // Nothing to do here.
    }

    OpenGLRenderingContext::~OpenGLRenderingContext() {
        delete _data;
    }
}
