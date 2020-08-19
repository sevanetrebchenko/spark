
#include <core/application.h>
#include <events/event.h>
#include <events/application_events.h>
#include <events/event_interactable.h>
#include <graphics/window/window.h>
#include <core/layer_stack.h>
#include <graphics/context/imgui_layer.h>
#include <utilitybox/logger/logger.h>

namespace Spark {
    //------------------------------------------------------------------------------------------------------------------
    // APPLICATION DATA
    //------------------------------------------------------------------------------------------------------------------
    class Application::ApplicationData : public Events::IEventReceivable<Application::ApplicationData, Events::WindowCloseEvent, Events::WindowResizeEvent> {
        public:
            ApplicationData();
            ~ApplicationData();
            void Run();

        private:
            // Allow attached IEventReceivable interface to access private OnEvent functions.
            friend class Events::IEventReceivable<Application::ApplicationData, Events::WindowCloseEvent, Events::WindowResizeEvent>;
            void OnEvent(Events::WindowCloseEvent* event) override;
            void OnEvent(Events::WindowResizeEvent* event) override;

            bool _running;
            float _deltaTime;

            Graphics::Window* _window;
            Graphics::ImGuiLayer* _imGuiLayer;
            LayerStack _layerStack;
    };

    Application::ApplicationData::ApplicationData() : _window(Graphics::Window::Create()), _imGuiLayer(Graphics::ImGuiLayer::Create(_window->GetNativeWindow())) {
        // Attach ImGui as an overlay.
        _layerStack.PushOverlay(_imGuiLayer);

        _running = true;
        _deltaTime = 0;
    }

    Application::ApplicationData::~ApplicationData() {
        delete _window;
    }

    void Application::ApplicationData::Run() {
        while(_running) {
            ServiceLocator::GetEventHub()->OnUpdate();
            static UtilityBox::Logger::LoggingSystem system {"lmao"};
            system.Log(UtilityBox::Logger::LogMessageSeverity::SEVERE, "testing");

            // Call OnUpdate for all layers.
            for (Layer* layer : _layerStack) {
                layer->OnUpdate(_deltaTime);
            }

//            // ImGui rendering.
//            _imGuiLayer->BeginFrame();
//            for (Layer* layer : _layerStack) {
//                layer->OnImGuiRender();
//            }
//            _imGuiLayer->EndFrame();

            _window->OnUpdate();
        }
    }

    void Application::ApplicationData::OnEvent(Events::WindowCloseEvent *event) {
    }

    void Application::ApplicationData::OnEvent(Events::WindowResizeEvent *event) {
    }


    //------------------------------------------------------------------------------------------------------------------
    // APPLICATION
    //------------------------------------------------------------------------------------------------------------------
    Application::Application() : _data(new ApplicationData()) {
        // Nothing to do here.
    }

    Application::~Application() {
        delete _data;
    }

    void Application::Run() {
        _data->Run();
    }
}
