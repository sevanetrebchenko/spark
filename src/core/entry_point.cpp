
#include <core/entry_point.h>
#include <events/event_hub.h>

#include <core/service_locator.h>
#include <ecs/systems/base_component_system.h>
// Services
#include <utilitybox/logger/logger.h> // ILoggingHub
#include <ecs/entities/entity_manager.h>

int main(int argc, char** argv) {
    auto* eventHub = Spark::Events::EventHub::GetInstance();
    auto* loggingHub = Spark::UtilityBox::Logger::LoggingHub::GetInstance();

    Spark::ServiceLocator::ProvideEventService(eventHub);
    Spark::ServiceLocator::ProvideLoggingService(loggingHub);

    auto* application = Spark::CreateApplication();

    application->Run();
    return 0;
}
