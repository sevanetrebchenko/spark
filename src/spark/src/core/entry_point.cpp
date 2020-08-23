
#include <spark/core/service_locator.h> // ServiceLocator
#include <events/event_hub.h>           // EventHub
#include <utilitybox/logger/logger.h>   // LoggingHub
#include <ecs/entities/entity_manager.h>   // LoggingHub
#include <core/entry_point.h>           // CreateApplication

int main(int argc, char** argv) {
    Spark::ServiceLocator::ProvideService(Spark::Events::EventHub::GetInstance());
    Spark::ServiceLocator::ProvideService(Spark::UtilityBox::Logger::LoggingHub::GetInstance());
    Spark::ServiceLocator::ProvideService(Spark::ECS::Entities::EntityManager::GetInstance());

    auto* application = Spark::CreateApplication();

    application->Run();
    return 0;
}
