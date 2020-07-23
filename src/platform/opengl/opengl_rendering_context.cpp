
#include <platform/opengl/opengl_rendering_context.h> // OpenGLRenderingContext
#include <platform/opengl/opengl_window.h>
#include <platform/opengl/imgui_overhead.h>
#include <utilitybox/logger/logging_system.h>

namespace Spark::Platform::OpenGL {

    class OpenGLRenderingContext::OpenGLRenderingContextData {
        public:
            explicit OpenGLRenderingContextData();
            ~OpenGLRenderingContextData();
            void Initialize();
            _NODISCARD_ const Graphics::Renderer::Window* GetWindow() const;
            _NODISCARD_ Graphics::Renderer::ImGuiOverhead* GetImGui() const;

        private:
            static void GLFWErrorCallback(int errorCode, const char* errorDescription);
            static void ParseErrorCode(UtilityBox::Logger::LogMessage& message, int errorCode);

            OpenGLWindow* _window;
            OpenGLImGuiOverhead* _imGuiOverhead;
            std::string _glslVersion;
            UtilityBox::Logger::LoggingSystem _loggingSystem;
    };

    OpenGLRenderingContext::OpenGLRenderingContextData::OpenGLRenderingContextData() : _window(new OpenGLWindow()), _imGuiOverhead(nullptr) {
        // Nothing to do here.
    }

    OpenGLRenderingContext::OpenGLRenderingContextData::~OpenGLRenderingContextData() {
        delete _imGuiOverhead;
        glfwTerminate();

        // Window deletion is not handled here.
        delete _window;
    }

    void OpenGLRenderingContext::OpenGLRenderingContextData::GLFWErrorCallback(int errorCode, const char* errorDescription) {
        // Static to this function only.
        static UtilityBox::Logger::LoggingSystem glfwErrorLoggingSystem { "GLFW Error" };

        UtilityBox::Logger::LogMessage message { UtilityBox::Logger::LogMessageSeverity::SEVERE };
        message.Supply("Entered OpenGL error callback function.");
        ParseErrorCode(message, errorCode);
        message.Supply("Provided error description: %s.", errorDescription);
        glfwErrorLoggingSystem.Log(message);
    }

    void OpenGLRenderingContext::OpenGLRenderingContextData::Initialize() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function Initialize.");
        message.Supply("Registering error callback function.");
        glfwSetErrorCallback(GLFWErrorCallback);

        // Initialize GLFW.
        int initializationCode = glfwInit();
        if (!initializationCode) {
            message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::SEVERE);
            message.Supply("GLFW initialization failed with error code: %i.", initializationCode);
            _loggingSystem.Log(message);
        }

        // TODO: Support higher version of OpenGL
        _glslVersion = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Initialize GLFW window.
        try {
            _window->Initialize();
        }
        catch (std::runtime_error& exception) {
            message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::SEVERE);
            message.Supply("Exception thrown: failed to initialize GLFW window.");
            _loggingSystem.Log(message);

            // Close GLFW.
            glfwTerminate();

            throw exception;
        }
        message.Supply("Successfully initialized GLFW window.");

        // Set current OpenGL context to this window.
        message.Supply("Current OpenGL context successfully set to this window.");
        glfwMakeContextCurrent(static_cast<GLFWwindow*>(_window->GetNativeWindow()));

        // Load Glad OpenGL extension (sets up OpenGL function pointers).
        int gladInitializationCode = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        // Failed to initialize Glad.
        if (!gladInitializationCode) {
            message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::SEVERE);
            message.Supply("Exception thrown: failed to initialize Glad. Error code: %i", gladInitializationCode);
            _loggingSystem.Log(message);

            delete _imGuiOverhead;
            glfwTerminate();
        }
        else {
            message.Supply("Successfully loaded and initialized Glad.");
        }

        // Initialize ImGui.
        // ImGui overhead requires an initialized window to be passed in.
        try {
            _imGuiOverhead = new OpenGLImGuiOverhead(static_cast<GLFWwindow*>(_window->GetNativeWindow()), _glslVersion.c_str());
        }
        catch (std::bad_alloc& exception) {
            message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::SEVERE);
            message.Supply("Exception thrown: failed to initialize ImGui overhead - program is out of memory.");
            _loggingSystem.Log(message);

            throw exception;
        }
        message.Supply("Successfully initialized ImGui overhead.");

        glfwSwapInterval(1);
    }

    void OpenGLRenderingContext::OpenGLRenderingContextData::ParseErrorCode(UtilityBox::Logger::LogMessage &message, int errorCode) {
        switch (errorCode) {
            case GLFW_NO_ERROR:
                message.Supply("GLFW error occurred: GLFW_NO_ERROR (error code: %i).", errorCode);
                break;
            case GLFW_NOT_INITIALIZED:
                message.Supply("GLFW error occurred: GLFW_NOT_INITIALIZED (error code: %i).", errorCode);
                break;
            case GLFW_NO_CURRENT_CONTEXT:
                message.Supply("GLFW error occurred: GLFW_NO_CURRENT_CONTEXT (error code: %i).", errorCode);
                break;
            case GLFW_INVALID_ENUM:
                message.Supply("GLFW error occurred: GLFW_INVALID_ENUM (error code: %i).", errorCode);
                break;
            case GLFW_INVALID_VALUE:
                message.Supply("GLFW error occurred: GLFW_INVALID_VALUE (error code: %i).", errorCode);
                break;
            case GLFW_OUT_OF_MEMORY:
                message.Supply("GLFW error occurred: GLFW_OUT_OF_MEMORY (error code: %i).", errorCode);
                break;
            case GLFW_API_UNAVAILABLE:
                message.Supply("GLFW error occurred: GLFW_API_UNAVAILABLE (error code: %i).", errorCode);
                break;
            case GLFW_VERSION_UNAVAILABLE:
                message.Supply("GLFW error occurred: GLFW_VERSION_UNAVAILABLE (error code: %i).", errorCode);
                break;
            case GLFW_PLATFORM_ERROR:
                message.Supply("GLFW error occurred: GLFW_PLATFORM_ERROR (error code: %i).", errorCode);
                break;
            case GLFW_FORMAT_UNAVAILABLE:
                message.Supply("GLFW error occurred: GLFW_FORMAT_UNAVAILABLE (error code: %i).", errorCode);
                break;
            case GLFW_NO_WINDOW_CONTEXT:
                message.Supply("GLFW error occurred: GLFW_NO_WINDOW_CONTEXT (error code: %i).", errorCode);
                break;
            default:
                break;
        }
    }

    const Graphics::Renderer::Window *OpenGLRenderingContext::OpenGLRenderingContextData::GetWindow() const {
        return _window;
    }

    Graphics::Renderer::ImGuiOverhead *OpenGLRenderingContext::OpenGLRenderingContextData::GetImGui() const {
        return _imGuiOverhead;
    }

    OpenGLRenderingContext::OpenGLRenderingContext() : RenderingContext(RenderingAPI::OPENGL), _data(new OpenGLRenderingContextData()) {
        // Nothing to do here.
    }

    OpenGLRenderingContext::~OpenGLRenderingContext() {
        delete _data;
    }

    void OpenGLRenderingContext::Initialize() {
        _data->Initialize();
    }

    const Graphics::Renderer::Window *OpenGLRenderingContext::GetWindow() const {
        return _data->GetWindow();
    }

    Graphics::Renderer::ImGuiOverhead *OpenGLRenderingContext::GetImGui() const {
        return _data->GetImGui();
    }
}
