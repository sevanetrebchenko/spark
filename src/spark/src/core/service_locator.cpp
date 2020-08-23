
#ifndef SPARK_SERVICE_LOCATOR_INL
#define SPARK_SERVICE_LOCATOR_INL

#include <spark/core/service_locator.h> // ServiceLocator

namespace Spark {
    class ServiceLocator::ServiceLocatorData {
        public:
            static void Initialize();
            static void ProvideService(Events::IEventHub* eventHubInterface);
            static void ProvideService(UtilityBox::Logger::ILoggingHub* loggingHubInterface);

            static Events::IEventHub* GetEventHub();
            static UtilityBox::Logger::ILoggingHub* GetLoggingHub();

        private:
            // Services.
            static Events::IEventHub* _eventHub;
            static UtilityBox::Logger::ILoggingHub* _loggingHub;
    };

    ServiceLocator::ServiceLocatorData* ServiceLocator::_data = nullptr;
    Events::IEventHub* ServiceLocator::ServiceLocatorData::_eventHub = nullptr;
    UtilityBox::Logger::ILoggingHub* ServiceLocator::ServiceLocatorData::_loggingHub = nullptr;

    void ServiceLocator::ServiceLocatorData::Initialize() {
    }

    void ServiceLocator::ServiceLocatorData::ProvideService(Events::IEventHub *eventHubInterface) {
        if (!_eventHub) {
            _eventHub = eventHubInterface;
        }
    }

    Events::IEventHub *ServiceLocator::ServiceLocatorData::GetEventHub() {
        // Return provided event hub service.
        return _eventHub;
    }

    void ServiceLocator::ServiceLocatorData::ProvideService(UtilityBox::Logger::ILoggingHub *loggingHubInterface) {
        if (!_loggingHub) {
            _loggingHub = loggingHubInterface;
        }
    }

    UtilityBox::Logger::ILoggingHub *ServiceLocator::ServiceLocatorData::GetLoggingHub() {
        return _loggingHub;
    }

    void ServiceLocator::Initialize() {
        _data->Initialize();
    }

    void ServiceLocator::ProvideEventService(Events::IEventHub *eventHubInterface) {
        _data->ProvideService(eventHubInterface);
    }

    void ServiceLocator::ProvideLoggingService(UtilityBox::Logger::ILoggingHub *loggingHubInterface) {
        _data->ProvideService(loggingHubInterface);
    }

    Events::IEventHub *ServiceLocator::GetEventHub() {
        return _data->GetEventHub();
    }

    UtilityBox::Logger::ILoggingHub *ServiceLocator::GetLoggingHub() {
        return _data->GetLoggingHub();
    }

}

#endif // SPARK_SERVICE_LOCATOR_INL
