
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
    class Application::ApplicationData : public UtilityBox::Logger::ILoggable, public Events::IEventReceivable<Application::ApplicationData, Events::WindowCloseEvent, Events::WindowResizeEvent, Events::WindowMinimizedEvent> {
        public:
            ApplicationData();
            ~ApplicationData();
            void Run();

        private:
            // Allow attached IEventReceivable interface to access private OnEvent functions.
            friend class Events::IEventReceivable<Application::ApplicationData, Events::WindowCloseEvent, Events::WindowResizeEvent, Events::WindowMinimizedEvent>;
            void OnEvent(Events::WindowCloseEvent* event) override;
            void OnEvent(Events::WindowResizeEvent* event) override;
            void OnEvent(Events::WindowMinimizedEvent* event) override;

            bool _running;
            bool _windowMinimized;
            float _deltaTime;

            Graphics::Window* _window;
            Graphics::ImGuiLayer* _imGuiLayer;
            LayerStack _layerStack;
    };

    Application::ApplicationData::ApplicationData() : UtilityBox::Logger::ILoggable("Application"),
                                                      IEventReceivable("Application"),
                                                      _window(Graphics::Window::Create()),
                                                      _imGuiLayer(Graphics::ImGuiLayer::Create(_window->GetNativeWindow()))
                                                      {
        // Attach ImGui as an overlay.
        _layerStack.PushOverlay(_imGuiLayer);

        _running = true;
        _windowMinimized = false;
        _deltaTime = 0;
    }

    Application::ApplicationData::~ApplicationData() {
        delete _window;
    }

    void Application::ApplicationData::Run() {
        auto* eventHub = dynamic_cast<Events::IEventHubPrivate*>(ServiceLocator::GetEventHub());

        while(_running) {
            eventHub->OnUpdate();

            if (!_windowMinimized) {
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
            }

            _window->OnUpdate();
        }
    }

    void Application::ApplicationData::OnEvent(Events::WindowCloseEvent *event) {
        _running = false;
    }

    void Application::ApplicationData::OnEvent(Events::WindowResizeEvent *event) {
    }

    void Application::ApplicationData::OnEvent(Events::WindowMinimizedEvent *event) {
        _windowMinimized = event->GetMinimized();
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
