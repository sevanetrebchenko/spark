
#include <core/entry_point.h>
#include <events/event_hub.h>
#include <events/event_listener.h>
#include <events/application_events.h>
#include <core/service_locator.h>
#include <utilitybox/logger/logger.h>

int main(int argc, char** argv) {
    auto* eventHub = Spark::Events::EventHub::GetInstance();
    auto* loggingHub = Spark::UtilityBox::Logger::LoggingHub::GetInstance();

    Spark::ServiceLocator::ProvideEventService(eventHub);
    Spark::ServiceLocator::ProvideLoggingService(loggingHub);

    auto* application = Spark::CreateApplication();

    eventHub->Dispatch(new Spark::Events::WindowCloseEvent());

    application->Run();
    return 0;
}
