
#ifndef SPARK_SERVICE_LOCATOR_INL
#define SPARK_SERVICE_LOCATOR_INL

#include <core/service_locator.h>
#include <events/null_event_hub.h>

namespace Spark {
    class ServiceLocator::ServiceLocatorData {
        public:
            static void Initialize();
            static void ProvideService(Events::IEventHub* eventHubInterface);

            static Events::IEventHub* GetEventHub();

        private:
            static Events::IEventHub* _eventHub;
    };

    ServiceLocator::ServiceLocatorData* ServiceLocator::_data = nullptr;
    Events::IEventHub* ServiceLocator::ServiceLocatorData::_eventHub = nullptr;

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

    void ServiceLocator::Initialize() {
        _data->Initialize();
    }

    void ServiceLocator::ProvideEventService(Events::IEventHub *eventHubInterface) {
        _data->ProvideService(eventHubInterface);
    }

    Events::IEventHub *ServiceLocator::GetEventHub() {
        return _data->GetEventHub();
    }

}

#endif // SPARK_SERVICE_LOCATOR_INL
