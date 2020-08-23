
#include <spark/core/application.h>                    // Application
#include <spark/core/service_locator.h>                // ServiceLocator
#include <events/event_hub_private_interface.h>        // IEventHubPrivate
#include <spark/utilitybox/logger/logging_interface.h> // ILoggable
#include <spark/events/event_interactable_interface.h> // IEventReceivable
#include <spark/events/types/application_events.h>     // WindowCloseEvent, WindowResizeEvent
#include <spark/graphics/window/window.h>              // Window
#include <spark/graphics/renderer/imgui_layer.h>       // ImGuiLayer
#include <core/layer_stack.h>                          // LayerStack

namespace Spark {
    //------------------------------------------------------------------------------------------------------------------
    // APPLICATION DATA
    //------------------------------------------------------------------------------------------------------------------
    class Application::ApplicationData : public UtilityBox::Logger::ILoggable, public Events::IEventReceivable<Application::ApplicationData, Events::WindowCloseEvent, Events::WindowResizeEvent> {
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

    Application::ApplicationData::ApplicationData() : _window(Graphics::Window::Create()), _imGuiLayer(Graphics::ImGuiLayer::Create(_window->GetNativeWindow())), UtilityBox::Logger::ILoggable("ILoggableInterfaceTest") {
        // Attach ImGui as an overlay.
        _layerStack.PushOverlay(_imGuiLayer);

        _running = true;
        _deltaTime = 0;
    }

    Application::ApplicationData::~ApplicationData() {
        delete _window;
    }

    void Application::ApplicationData::Run() {
        auto* eventHub = dynamic_cast<Events::IEventHubPrivate*>(ServiceLocator::GetEventHub());

        while(_running) {
            eventHub->OnUpdate();

            LogError("testing testing testing testing testing testing testing testing testing testing testing ");

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
