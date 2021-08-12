
#include <spark/events/event_hub.h>            // EventHub
#include <spark/logger/logger.h>    // LoggingHub
#include <spark/ecs/entities/entity_manager.h> // EntityManager
#include <spark/core/application.h>            // CreateApplication
#include <spark/logger/adapter/types/file_adapter.h> // FileAdapter

#include <spark/memory/allocator.h>

#include <spark/ecs/systems/base_component_system.h>

#define ALLOC(TYPE, RESOURCE) Spark::UtilityBox::Memory::IAllocator<TYPE, Spark::UtilityBox::Memory::RESOURCE>

struct TestSystem1 : public Spark::Events::EventListener<TestSystem1, Spark::Events::EntityCreatedEvent, Spark::Events::EntityDestroyedEvent> {
    void OnEvent(const Spark::Events::EntityCreatedEvent* event) override {
        std::cout << "TestSystem1: Received entity created event with ID: " << event->GetEntityID() << std::endl;
    }

    void OnEvent(const Spark::Events::EntityDestroyedEvent* event) override {
        std::cout << "TestSystem1: Received entity destroyed event with ID: " << event->GetEntityID() << std::endl;
    }
};

struct TestSystem2 : public Spark::Events::EventListener<TestSystem2, Spark::Events::EntityCreatedEvent, Spark::Events::EntityDestroyedEvent> {
    void OnEvent(const Spark::Events::EntityCreatedEvent* event) override {
        std::cout << "TestSystem2: Received entity created event with ID: " << event->GetEntityID() << std::endl;
    }

    void OnEvent(const Spark::Events::EntityDestroyedEvent* event) override {
        std::cout << "TestSystem2: Received entity destroyed event with ID: " << event->GetEntityID() << std::endl;
    }
};


int main(int argc, char** argv) {
    Spark::Singleton<Spark::Logger::LoggingHub>::GetInstance()->AddAdapter(new Spark::Logger::FileAdapter("log.txt", Spark::Logger::AdapterConfiguration{"Log"}));

    Spark::ECS::EntityManager* em = Spark::Singleton<Spark::ECS::EntityManager>::GetInstance();
    TestSystem1 ts1;
    TestSystem2 ts2;

    em->CreateEntity("test entity 1");
    em->CreateEntity("test entity 2");
    em->CreateEntity("test entity 3");

    Spark::Singleton<Spark::Events::EventHub>::GetInstance()->OnUpdate(0);

    em->DestroyEntity("test entity 3");
    em->DestroyEntity("test entity 2");
    em->DestroyEntity("test entity 1");

    Spark::Singleton<Spark::Events::EventHub>::GetInstance()->OnUpdate(0);

//    Spark::ECS::EntityManager a;
//
//    //Spark::ECS::BaseComponentSystem<> a("ehe");
//
//    // Setup services for service locator.
//    Spark::ServiceLocator::ProvideService(Spark::Events::EventHub::GetInstance());
//
//    // Set up custom adapters.
//    Spark::LoggingHub::LoggingHub::LoggingHub* loggingHub = Spark::LoggingHub::LoggingHub::LoggingHub::GetInstance();
//    // Debug file adapter.
//    auto* debugFileAdapter = new Spark::LoggingHub::LoggingHub::FileAdapter("debug_log.txt");
//    debugFileAdapter->GetConfiguration()->SetMessageSeverityCutoff(Spark::LoggingHub::LoggingHub::LogSeverity::DEBUG);
//
//    // Warning file adapter.
//    auto* warningFileAdapter = new Spark::LoggingHub::LoggingHub::FileAdapter("warning_log.txt");
//    warningFileAdapter->GetConfiguration()->SetMessageSeverityCutoff(Spark::LoggingHub::LoggingHub::LogSeverity::WARNING);
//
//    // Error file adapter.
//    auto* errorFileAdapter = new Spark::LoggingHub::LoggingHub::FileAdapter("error_log.txt");
//    errorFileAdapter->GetConfiguration()->SetMessageSeverityCutoff(Spark::LoggingHub::LoggingHub::LogSeverity::ERROR);
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
