
#include <core/application.h>
#include <events/event.h>
#include <events/event_listener.h>
#include <events/application_events.h>
#include <events/event_interactable.h>
#include <graphics/window/window.h>
#include <utilitybox/tools/utility_functions.h>

namespace Spark {
    //------------------------------------------------------------------------------------------------------------------
    // APPLICATION DATA
    //------------------------------------------------------------------------------------------------------------------
    class Application::ApplicationData : public Events::IEventReceivable<Events::WindowCloseEvent, Events::WindowResizeEvent> {
        public:
            ApplicationData();
            ~ApplicationData();
            void Run();

        public:
            // Event processing.
            void OnEvent(Events::WindowCloseEvent* event) override {
                std::cout << event->ToString() << std::endl;
            }

            void OnEvent(Events::WindowResizeEvent* event) override {
                std::cout << event->ToString() << std::endl;
            }

            bool _running;
            Graphics::Window* _window;
    };

    Application::ApplicationData::ApplicationData() : _window(Graphics::Window::Create()) {
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
    }

    Application::~Application() {
        delete _data;
    }

    void Application::Run() {
        _data->Run();
    }
}
