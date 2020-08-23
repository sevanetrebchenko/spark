
#include <spark/core/service_locator.h> // ServiceLocator
#include <events/event_hub.h>           // EventHub
#include <utilitybox/logger/logger.h>   // LoggingHub
#include <core/entry_point.h>           // CreateApplication

int main(int argc, char** argv) {
    auto* eventHub = Spark::Events::EventHub::GetInstance();
    auto* loggingHub = Spark::UtilityBox::Logger::LoggingHub::GetInstance();

    Spark::ServiceLocator::ProvideEventService(eventHub);
    Spark::ServiceLocator::ProvideLoggingService(loggingHub);

    auto* application = Spark::CreateApplication();

    application->Run();
    return 0;
}
