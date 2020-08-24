
#include <spark/core/service_locator.h>  // ServiceLocator
#include <events/event_hub.h>            // EventHub
#include <utilitybox/logger/logger.h>    // LoggingHub
#include <ecs/entities/entity_manager.h> // EntityManager
#include <core/entry_point.h>            // CreateApplication
#include <spark/events/types/mouse_events.h> // MouseEvent
#include <spark/ecs/systems/base_component_system.h>

int main(int argc, char** argv) {
    Spark::ServiceLocator::ProvideService(Spark::Events::EventHub::GetInstance());
    Spark::ServiceLocator::ProvideService(Spark::UtilityBox::Logger::LoggingHub::GetInstance());
    Spark::ServiceLocator::ProvideService(Spark::ECS::Entities::EntityManager::GetInstance());

    Spark::ServiceLocator::GetEventHub()->Dispatch(new Spark::Events::MouseButtonPressedEvent(4));

    auto* application = Spark::CreateApplication();

    application->Run();
    return 0;
}
