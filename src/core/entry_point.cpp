
#include <core/entry_point.h>
#include <events/event_hub.h>
#include <events/event_listener.h>
#include <events/application_events.h>

int main(int argc, char** argv) {
    auto* application = Spark::CreateApplication();

    auto* eventHub = Spark::Events::EventHub::GetInstance();
    auto* listener = new Spark::Events::EventListener<Spark::Events::WindowCloseEvent, Spark::Events::WindowResizeEvent>();
    auto* listener2 = new Spark::Events::EventListener<Spark::Events::WindowResizeEvent>();
    eventHub->AttachEventListener(listener);
    eventHub->AttachEventListener(listener2);
    eventHub->Dispatch(new Spark::Events::WindowResizeEvent(100, 100));
    eventHub->Dispatch(new Spark::Events::WindowCloseEvent());
    eventHub->DetachEventListener(listener);
    eventHub->DetachEventListener(listener);

    return 0;
}
