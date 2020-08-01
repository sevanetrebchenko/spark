
#include <platform/opengl/opengl_rendering_context.h> // OpenGLRenderingContext
#include <platform/opengl/opengl_window.h>
#include <platform/opengl/opengl_imgui_overhead.h>
#include <utilitybox/logger/logging_system.h>
#include <graphics/context/rendering_api.h>

namespace Spark::Platform::OpenGL {

    //------------------------------------------------------------------------------------------------------------------
    // OPENGL RENDERING CONTEXT DATA
    //------------------------------------------------------------------------------------------------------------------
    class OpenGLRenderingContext::OpenGLRenderingContextData {
        public:
            /**
             * Back-end initialization for managing the OpenGL context. Takes double pointers to both window and ImGui
             * overhead objects that are owned by the base RenderingContext class for initialization.
             * @param window        - Window (owned by base RenderingContext) to initialize to an OpenGL window.
             * @param imGuiOverhead - ImGui overhead (owned by base RenderingContext) to initialize to be OpenGL specific.
             */
            explicit OpenGLRenderingContextData(Graphics::Context::Window** window, Graphics::Context::ImGuiOverhead** imGuiOverhead, UtilityBox::Logger::LoggingSystem* loggingSystem);

            /**
             * Cleans up memory associated with OpenGL rendering context overhead. Calls destructors on the OpenGL portion
             * on objects shared with the base RenderingContext to prevent memory leaks.
             */
            ~OpenGLRenderingContextData();

        private:
            /**
             * Callback on GLFW error.
             * @param errorCode        - Error code.
             * @param errorDescription - Error description.
             */
            static void GLFWErrorCallback(int errorCode, const char* errorDescription);

            /**
             * Helper function to parse error code and log it through the system's logging system.
             * @param message   - Message to use while logging.
             * @param errorCode - Error code.
             */
            static void ParseErrorCode(UtilityBox::Logger::LogMessage& message, int errorCode);

            void InitializeGLFW(UtilityBox::Logger::LogMessage& message);
            void InitializeGlad(UtilityBox::Logger::LogMessage& message);
            void InitializeOpenGLWindow(UtilityBox::Logger::LogMessage& message, Graphics::Context::Window** window);
            void InitializeOpenGLContext(UtilityBox::Logger::LogMessage& message);
            void InitializeImGui(UtilityBox::Logger::LogMessage& message, Graphics::Context::ImGuiOverhead** imGuiOverhead);

            OpenGLWindow* _window;               // Window object (not owned by this class).
            OpenGLImGuiOverhead* _imGuiOverhead; // ImGui overhead object (not owned by this class).

            UtilityBox::Logger::LoggingSystem* _loggingSystem; // Logging system.
    };

    OpenGLRenderingContext::OpenGLRenderingContextData::OpenGLRenderingContextData(Graphics::Context::Window** window, Graphics::Context::ImGuiOverhead** imGuiOverhead, UtilityBox::Logger::LoggingSystem* loggingSystem) : _loggingSystem(loggingSystem) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function Initialize.");
        glfwSetErrorCallback(GLFWErrorCallback);
        message.Supply("Registered GLFW error callback function.");

        // Initialize GLFW.
        InitializeGLFW(message); // Exception not caught on purpose.

        // Initialize window.
        try {
            InitializeOpenGLWindow(message, window);
        }
        // Need to terminate GLFW before returning from function.
        catch (std::runtime_error& runtimeError) {
            glfwTerminate();

            // Message has already been logged.
            throw runtimeError;
        }
        catch (std::bad_alloc& badAlloc) {
            glfwTerminate();

            // Message has already been logged.
            throw badAlloc;
        }

        InitializeOpenGLContext(message);

        // Load Glad OpenGL extension (sets up OpenGL function pointers).
        try {
            InitializeGlad(message);
        }
            // Need to terminate GLFW before returning from function.
        catch (std::runtime_error& runtimeError) {
            glfwTerminate();

            // Message has already been logged.
            throw runtimeError;
        }

        // Initialize ImGui.
        try {
            InitializeImGui(message, imGuiOverhead);
        }
        catch (std::bad_alloc& badAlloc) {
            // Destroy OpenGL portion of window. Base destructor cleans up the remaining window data.
            _window->~OpenGLWindow();

            // Terminate GLFW processing.
            glfwTerminate();

            // Message has already been logged.
            throw badAlloc;
        }

        glfwSwapInterval(1);
    }

    OpenGLRenderingContext::OpenGLRenderingContextData::~OpenGLRenderingContextData() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering destructor for OpenGLContext.");

        delete _window;
        delete _imGuiOverhead;
        glfwTerminate();

        message.Supply("Destructor finished.");

        _loggingSystem->Log(message);
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

    void OpenGLRenderingContext::OpenGLRenderingContextData::InitializeGLFW(UtilityBox::Logger::LogMessage &message) {
        int initializationCode = glfwInit();
        // Failed to initialize GLFW
        if (!initializationCode) {
            message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::SEVERE);
            message.Supply("GLFW initialization failed with error code: %i.", initializationCode);
            _loggingSystem->Log(message);

            throw std::runtime_error("GLFW initialization failed.");
        }
        else {
            message.Supply("GLFW successfully initialized.");
        }
    }

    void OpenGLRenderingContext::OpenGLRenderingContextData::InitializeGlad(UtilityBox::Logger::LogMessage &message) {
        int initializationCode = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        // Failed to initialize Glad.
        if (!initializationCode) {
            message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::SEVERE);
            message.Supply("Exception thrown: failed to initialize Glad. Error code: %i", initializationCode);
            _loggingSystem->Log(message);

            throw std::runtime_error("Glad initialization failed.");
        }
        else {
            message.Supply("Successfully loaded and initialized Glad.");
        }
    }

    void OpenGLRenderingContext::OpenGLRenderingContextData::InitializeOpenGLWindow(UtilityBox::Logger::LogMessage &message, Graphics::Context::Window **window) {
        // TODO: Support higher version of OpenGL
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Initialize GLFW window.
        try {
            *window = new OpenGLWindow();
        }
        catch (std::runtime_error& runtimeError) {
            message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::SEVERE);
            message.Supply("Exception thrown: Failed to initialize OpenGL window. Provided error message: '%s'.", runtimeError.what());
            _loggingSystem->Log(message);

            throw runtimeError;
        }
        catch (std::bad_alloc& badAlloc) {
            message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::SEVERE);
            message.Supply("Exception thrown: Failed to allocate enough memory for OpenGL window.");
            _loggingSystem->Log(message);

            throw badAlloc;
        }
        message.Supply("OpenGL window successfully created.");

        _window = dynamic_cast<OpenGLWindow*>(*window); // Should always succeed.
    }

    void OpenGLRenderingContext::OpenGLRenderingContextData::InitializeImGui(UtilityBox::Logger::LogMessage &message, Graphics::Context::ImGuiOverhead **imGuiOverhead) {
        // ImGui overhead requires an initialized window to be passed in.
        try {
            *imGuiOverhead = new OpenGLImGuiOverhead(_window);
        }
        catch (std::bad_alloc& badAlloc) {
            message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::SEVERE);
            message.Supply("Exception thrown: Failed to allocate enough memory for OpenGL ImGui overhead.");
            _loggingSystem->Log(message);

            throw badAlloc;
        }
        _imGuiOverhead = dynamic_cast<OpenGLImGuiOverhead*>(*imGuiOverhead);
        message.Supply("Successfully initialized ImGui overhead.");
    }

    void OpenGLRenderingContext::OpenGLRenderingContextData::InitializeOpenGLContext(UtilityBox::Logger::LogMessage &message) {
        // Set current OpenGL context to this window.
        glfwMakeContextCurrent(static_cast<GLFWwindow*>(_window->GetNativeWindow()));
        message.Supply("Current OpenGL context successfully set to this window.");
    }

    //------------------------------------------------------------------------------------------------------------------
    // OPENGL RENDERING CONTEXT
    //------------------------------------------------------------------------------------------------------------------
    OpenGLRenderingContext::OpenGLRenderingContext() : RenderingContext(Graphics::Context::RenderingAPI::OPENGL), _data(new OpenGLRenderingContextData(&_window, &_imGuiOverhead, &_loggingSystem)) {
        // Nothing to do here.
    }

    OpenGLRenderingContext::~OpenGLRenderingContext() {
        delete _data;
    }
}
