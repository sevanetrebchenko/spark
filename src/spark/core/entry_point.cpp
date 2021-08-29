
#include <spark/events/event_hub.h>            // EventHub
#include <spark/logger/logger.h>    // LoggingHub
#include <spark/ecs/entities/entity_manager.h> // EntityManager
#include <spark/core/application.h>            // CreateApplication
#include <spark/logger/adapter/types/file_adapter.h> // FileAdapter

#include <spark/memory/allocator.h>
#include <spark/memory/allocators/segmented_pool_allocator.h>

#include <spark/ecs/systems/base_component_system.h>
#include "spark/job/job_system.h"
#include "spark/job/job_handle_manager.h"

#define ALLOC(TYPE, RESOURCE) Spark::UtilityBox::Memory::IAllocator<TYPE, Spark::UtilityBox::Memory::RESOURCE>

struct TestSystem1 : public Spark::Events::EventListener<TestSystem1, Spark::Events::EntityCreatedEvent, Spark::Events::EntityDestroyedEvent> {
    void OnEvent(const Spark::Events::EntityCreatedEvent* event) override {
        LogDebug("TestSystem1: Received entity created event with ID: %u", event->GetEntityID());
    }

    void OnEvent(const Spark::Events::EntityDestroyedEvent* event) override {
        LogDebug("TestSystem1: Received entity destroyed event with ID: %u", event->GetEntityID());
    }
};

struct TestSystem2 : public Spark::Events::EventListener<TestSystem2, Spark::Events::EntityCreatedEvent, Spark::Events::EntityDestroyedEvent> {
    void OnEvent(const Spark::Events::EntityCreatedEvent* event) override {
        LogDebug("TestSystem2: Received entity created event with ID: %u", event->GetEntityID());
    }

    void OnEvent(const Spark::Events::EntityDestroyedEvent* event) override {
        LogDebug("TestSystem2: Received entity destroyed event with ID: %u", event->GetEntityID());
    }
};

using namespace Spark::Job;

// No dependency, single scope.
void HandleTest1() {
    std::cout << "Test 1" << std::endl;
    ManagedJobHandle handle1 = Spark::Singleton<JobSystem>::GetInstance()->Schedule<Test>(5);
}

// No dependency, outside scope.
ManagedJobHandle HandleTest2() {
    std::cout << "Test 2" << std::endl;
    return Spark::Singleton<JobSystem>::GetInstance()->Schedule<Test>(5);
}

// Single dependency, single scope.
void HandleTest3() {
    std::cout << "Test 3" << std::endl;
    ManagedJobHandle handle1 = Spark::Singleton<JobSystem>::GetInstance()->Schedule<Test>(5);
    ManagedJobHandle handle2 = Spark::Singleton<JobSystem>::GetInstance()->Schedule<Test>(8);

    handle2->AddDependency(handle1);
}

// Single dependency, outside scope.
ManagedJobHandle HandleTest4() {
    std::cout << "Test 4" << std::endl;
    ManagedJobHandle handle1 = Spark::Singleton<JobSystem>::GetInstance()->Schedule<Test>(5);
    ManagedJobHandle handle2 = Spark::Singleton<JobSystem>::GetInstance()->Schedule<Test>(8);

    handle2->AddDependency(handle1);
    return handle2;
}

int main(int argc, char** argv) {
    Spark::Logger::TimeStamp::Init();
    Spark::Singleton<Spark::Logger::LoggingHub>::GetInstance()->AddAdapter(new Spark::Logger::FileAdapter("log.txt", Spark::Logger::AdapterConfiguration{"Log"}));

    {
        HandleTest1();
    }


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


    std::cout << "end of main" << std::endl;
    std::cout.flush();
    std::cerr.flush();
    return 0;
}
