
#include <spark/core/service_locator.h> // ServiceLocator

namespace Spark {
    //------------------------------------------------------------------------------------------------------------------
    // SERVICE LOCATOR DATA
    //------------------------------------------------------------------------------------------------------------------
    class ServiceLocator::ServiceLocatorData {
        public:
            static void ProvideService(Events::IEventHub* eventHubInterface);
            static void ProvideService(UtilityBox::Logger::ILoggingHub* loggingHubInterface);
            static void ProvideService(ECS::Entities::IEntityManager* entityManagerInterface);

            static Events::IEventHub* GetEventHub();
            static UtilityBox::Logger::ILoggingHub* GetLoggingHub();
            static ECS::Entities::IEntityManager* GetEntityManager();

        private:
            // Services.
            static Events::IEventHub* _eventHub;
            static UtilityBox::Logger::ILoggingHub* _loggingHub;
            static ECS::Entities::IEntityManager* _entityManager;
    };

    ServiceLocator::ServiceLocatorData* ServiceLocator::_data = nullptr;
    Events::IEventHub* ServiceLocator::ServiceLocatorData::_eventHub = nullptr;
    UtilityBox::Logger::ILoggingHub* ServiceLocator::ServiceLocatorData::_loggingHub = nullptr;
    ECS::Entities::IEntityManager* ServiceLocator::ServiceLocatorData::_entityManager = nullptr;

    void ServiceLocator::ServiceLocatorData::ProvideService(Events::IEventHub *eventHubInterface) {
        if (!_eventHub) {
            _eventHub = eventHubInterface;
        }
    }

    void ServiceLocator::ServiceLocatorData::ProvideService(UtilityBox::Logger::ILoggingHub *loggingHubInterface) {
        if (!_loggingHub) {
            _loggingHub = loggingHubInterface;
        }
    }

    void ServiceLocator::ServiceLocatorData::ProvideService(ECS::Entities::IEntityManager *entityManagerInterface) {
        if (!_entityManager) {
            _entityManager = entityManagerInterface;
        }
    }

    Events::IEventHub *ServiceLocator::ServiceLocatorData::GetEventHub() {
        // Return provided event hub service.
        return _eventHub;
    }

    //------------------------------------------------------------------------------------------------------------------
    // SERVICE LOCATOR
    //------------------------------------------------------------------------------------------------------------------
    UtilityBox::Logger::ILoggingHub *ServiceLocator::ServiceLocatorData::GetLoggingHub() {
        return _loggingHub;
    }

    ECS::Entities::IEntityManager *ServiceLocator::ServiceLocatorData::GetEntityManager() {
        return _entityManager;
    }

    void ServiceLocator::ProvideService(Events::IEventHub *eventHubInterface) {
        _data->ProvideService(eventHubInterface);
    }

    void ServiceLocator::ProvideService(UtilityBox::Logger::ILoggingHub *loggingHubInterface) {
        _data->ProvideService(loggingHubInterface);
    }

    void ServiceLocator::ProvideService(ECS::Entities::IEntityManager *entityManagerInterface) {
        _data->ProvideService(entityManagerInterface);
    }

    Events::IEventHub *ServiceLocator::GetEventHub() {
        return _data->GetEventHub();
    }

    UtilityBox::Logger::ILoggingHub *ServiceLocator::GetLoggingHub() {
        return _data->GetLoggingHub();
    }

    ECS::Entities::IEntityManager *ServiceLocator::GetEntityManager() {
        return _data->GetEntityManager();
    }

}
