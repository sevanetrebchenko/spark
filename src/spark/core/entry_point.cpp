
#include <spark/core/service_locator.h>        // ServiceLocator
#include <spark/events/event_hub.h>            // EventHub
#include <spark/utilitybox/logger/logger.h>    // LoggingHub
#include <spark/ecs/entities/entity_manager.h> // EntityManager
#include <spark/core/application.h>            // CreateApplication

int main(int argc, char** argv) {
    // Setup services for service locator.
    Spark::ServiceLocator::ProvideService(Spark::Events::EventHub::GetInstance());

    Spark::UtilityBox::Logger::LoggingHub* loggingHub = Spark::UtilityBox::Logger::LoggingHub::GetInstance();


    Spark::ServiceLocator::ProvideService(Spark::UtilityBox::Logger::LoggingHub::GetInstance());
    Spark::ServiceLocator::ProvideService(Spark::ECS::Entities::EntityManager::GetInstance());

    // Application.
    auto* application = Spark::CreateApplication();
    application->Run();
    delete application;

    return 0;
}
