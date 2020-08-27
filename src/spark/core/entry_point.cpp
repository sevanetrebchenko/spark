
#include <spark/core/service_locator.h>        // ServiceLocator
#include <spark/events/event_hub.h>            // EventHub
#include <spark/utilitybox/logger/logger.h>    // LoggingHub
#include <spark/ecs/entities/entity_manager.h> // EntityManager
#include <spark/core/application.h>            // CreateApplication
#include <spark/utilitybox/logger/adapter/types/file_adapter.h> // FileAdapter

int main(int argc, char** argv) {
    // Setup services for service locator.
    Spark::ServiceLocator::ProvideService(Spark::Events::EventHub::GetInstance());

    // Set up custom adapters.
    Spark::UtilityBox::Logger::LoggingHub* loggingHub = Spark::UtilityBox::Logger::LoggingHub::GetInstance();
    // Debug file adapter.
    auto* debugFileAdapter = new Spark::UtilityBox::Logger::FileAdapter("debug_log.txt");
    debugFileAdapter->GetConfiguration()->SetMessageSeverityCutoff(Spark::UtilityBox::Logger::LogMessageSeverity::DEBUG);

    // Warning file adapter.
    auto* warningFileAdapter = new Spark::UtilityBox::Logger::FileAdapter("warning_log.txt");
    warningFileAdapter->GetConfiguration()->SetMessageSeverityCutoff(Spark::UtilityBox::Logger::LogMessageSeverity::WARNING);

    // Error file adapter.
    auto* errorFileAdapter = new Spark::UtilityBox::Logger::FileAdapter("error_log.txt");
    errorFileAdapter->GetConfiguration()->SetMessageSeverityCutoff(Spark::UtilityBox::Logger::LogMessageSeverity::ERROR);

    loggingHub->AttachCustomAdapter(debugFileAdapter);
    loggingHub->AttachCustomAdapter(warningFileAdapter);
    loggingHub->AttachCustomAdapter(errorFileAdapter);

    Spark::ServiceLocator::ProvideService(loggingHub);
    Spark::ServiceLocator::ProvideService(Spark::ECS::Entities::EntityManager::GetInstance());

    // Application.
    auto* application = Spark::CreateApplication();
    application->Run();
    delete application;

    return 0;
}
