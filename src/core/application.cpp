
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

        private:
            void ProcessEvents(std::queue<std::shared_ptr<Events::Event*>>& eventData) override;
            void ProcessEvent(Events::Event* event);

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

    void Application::ApplicationData::ProcessEvents(std::queue<std::shared_ptr<Events::Event*>>& eventData) {
        while (!eventData.empty()) {
            Events::Event* eventPtr = *eventData.front();
            std::cout << eventPtr->ToString() << std::endl;

            ProcessEvent(eventPtr);
            eventData.pop();
        }
    }

    void Application::ApplicationData::ProcessEvent(Events::Event* event) {
        if (Events::WindowCloseEvent* windowCloseEvent = dynamic_cast<Events::WindowCloseEvent*>(event)) {
            _running = false;
        }

        if (Events::WindowResizeEvent* windowResizeEvent = dynamic_cast<Events::WindowResizeEvent*>(event) ) {
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
