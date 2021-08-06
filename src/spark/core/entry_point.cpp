
#include <spark/core/service_locator.h>        // ServiceLocator
#include <spark/events/event_hub.h>            // EventHub
#include <spark/utilitybox/logger/logger.h>    // LoggingHub
#include <spark/ecs/entities/entity_manager.h> // EntityManager
#include <spark/core/application.h>            // CreateApplication
#include <spark/utilitybox/logger/adapter/types/file_adapter.h> // FileAdapter

#include <spark/utilitybox/memory/allocator.h>

#include <spark/ecs/systems/base_component_system.h>

#define ALLOC(TYPE, RESOURCE) Spark::UtilityBox::Memory::IAllocator<TYPE, Spark::UtilityBox::Memory::RESOURCE>

int main(int argc, char** argv) {

    std::vector<int, Spark::UtilityBox::Memory::IAllocator<int, Spark::UtilityBox::Memory::IMemoryResource1>> vec1;
    std::vector<int, Spark::UtilityBox::Memory::IAllocator<int, Spark::UtilityBox::Memory::IMemoryResource2>> vec2;

    Spark::UtilityBox::Memory::IAllocator<int, Spark::UtilityBox::Memory::IMemoryResource1> alloc1;
    Spark::UtilityBox::Memory::IAllocator<int, Spark::UtilityBox::Memory::IMemoryResource2> alloc2;

    Spark::ECS::EntityManager a;
//
//    //Spark::ECS::BaseComponentSystem<> a("ehe");
//
//    // Setup services for service locator.
//    Spark::ServiceLocator::ProvideService(Spark::Events::EventHub::GetInstance());
//
//    // Set up custom adapters.
//    Spark::UtilityBox::Logger::LoggingHub* loggingHub = Spark::UtilityBox::Logger::LoggingHub::GetInstance();
//    // Debug file adapter.
//    auto* debugFileAdapter = new Spark::UtilityBox::Logger::FileAdapter("debug_log.txt");
//    debugFileAdapter->GetConfiguration()->SetMessageSeverityCutoff(Spark::UtilityBox::Logger::LogSeverity::DEBUG);
//
//    // Warning file adapter.
//    auto* warningFileAdapter = new Spark::UtilityBox::Logger::FileAdapter("warning_log.txt");
//    warningFileAdapter->GetConfiguration()->SetMessageSeverityCutoff(Spark::UtilityBox::Logger::LogSeverity::WARNING);
//
//    // Error file adapter.
//    auto* errorFileAdapter = new Spark::UtilityBox::Logger::FileAdapter("error_log.txt");
//    errorFileAdapter->GetConfiguration()->SetMessageSeverityCutoff(Spark::UtilityBox::Logger::LogSeverity::ERROR);
//
//    loggingHub->AttachCustomAdapter(debugFileAdapter);
//    loggingHub->AttachCustomAdapter(warningFileAdapter);
//    loggingHub->AttachCustomAdapter(errorFileAdapter);
//
//    Spark::ServiceLocator::ProvideService(loggingHub);
//    Spark::ServiceLocator::ProvideService(Spark::ECS::EntityManager::GetInstance());
//
//    // Application.
//    auto* application = Spark::CreateApplication();
//    application->Run();
//    delete application;

    return 0;
}
