
#include <core/application.h>
#include <events/event.h>
#include <events/application_events.h>
#include <events/event_interactable.h>
#include <graphics/window/window.h>
#include <core/layer_stack.h>
#include <graphics/context/imgui_layer.h>

namespace Spark {
    //------------------------------------------------------------------------------------------------------------------
    // APPLICATION DATA
    //------------------------------------------------------------------------------------------------------------------
    class Application::ApplicationData : public Events::IEventReceivable<Application::ApplicationData, Events::WindowCloseEvent, Events::WindowResizeEvent> {
        public:
            ApplicationData();
            ~ApplicationData();
            void Run();

            void OnEvent(Events::WindowCloseEvent* event) override {
                if (IEventReceivable::CheckEventPointer(event)) {
                    std::cout << "success" << std::endl;
                }
                else {
                    std::cout << "nop" << std::endl;
                }
            }
            void OnEvent(Events::WindowResizeEvent* event) override {

            }

        private:
            bool _running;
            Graphics::Window* _window;
            Graphics::ImGuiLayer* _imGuiLayer;
            LayerStack _layerStack;
    };

    Application::ApplicationData::ApplicationData() : _window(Graphics::Window::Create()), _imGuiLayer(Graphics::ImGuiLayer::Create(_window->GetNativeWindow())) {
        // Attach ImGui as an overlay.
        _layerStack.PushOverlay(_imGuiLayer);
        _running = true;
    }

    Application::ApplicationData::~ApplicationData() {
        delete _window;
    }

    void Application::ApplicationData::Run() {
        while(_running) {
            IEventReceivable::OnUpdate();
        }
    }


    //------------------------------------------------------------------------------------------------------------------
    // APPLICATION
    //------------------------------------------------------------------------------------------------------------------
    Application::Application() : _data(new ApplicationData()) {
        // Nothing to do here.
        _data->OnEvent(new Events::WindowCloseEvent());
    }

    Application::~Application() {
        delete _data;
    }

    void Application::Run() {
        _data->Run();
    }
}
