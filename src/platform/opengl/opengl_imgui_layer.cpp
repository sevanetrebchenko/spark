
#include <platform/opengl/opengl_imgui_layer.h>
#include <graphics/context/imgui_overhead.h>

namespace Spark::Graphics {
    //------------------------------------------------------------------------------------------------------------------
    // OPENGL IMGUI LAYER DATA
    //------------------------------------------------------------------------------------------------------------------
    class OpenGLImGuiLayer::OpenGLImGuiLayerData {
        public:
            explicit OpenGLImGuiLayerData(GLFWwindow* window);
            ~OpenGLImGuiLayerData();

            void BeginFrame();
            void EndFrame();

        private:
            ImGuiOverhead* _imGuiOverhead;
    };

    OpenGLImGuiLayer::OpenGLImGuiLayerData::OpenGLImGuiLayerData(GLFWwindow* window) : _imGuiOverhead(Graphics::ImGuiOverhead::Create(window)) {
        // Nothing to do here.
    }

    OpenGLImGuiLayer::OpenGLImGuiLayerData::~OpenGLImGuiLayerData() {
        delete _imGuiOverhead;
    }

    void OpenGLImGuiLayer::OpenGLImGuiLayerData::BeginFrame() {
        _imGuiOverhead->BeginFrame();
    }

    void OpenGLImGuiLayer::OpenGLImGuiLayerData::EndFrame() {
        _imGuiOverhead->EndFrame();
    }


    //------------------------------------------------------------------------------------------------------------------
    // OPENGL IMGUI LAYER
    //------------------------------------------------------------------------------------------------------------------
    OpenGLImGuiLayer::OpenGLImGuiLayer(GLFWwindow* window) : _data(new OpenGLImGuiLayerData(window)) {
        // Nothing to do here.
    }

    OpenGLImGuiLayer::~OpenGLImGuiLayer() {
        delete _data;
    }

    void OpenGLImGuiLayer::OnAttach() {
    }

    void OpenGLImGuiLayer::OnDetach() {
    }

    void OpenGLImGuiLayer::BeginFrame() {
        _data->BeginFrame();
    }

    void OpenGLImGuiLayer::EndFrame() {
        _data->EndFrame();
    }
}
