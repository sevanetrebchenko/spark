
#ifndef SPARK_SERVICE_LOCATOR_H
#define SPARK_SERVICE_LOCATOR_H

#include <events/event_hub_interface.h>

namespace Spark {

    class ServiceLocator {
        public:
            static void Initialize();
            static void ProvideEventService(Events::IEventHub* eventHubInterface);
            static Events::IEventHub* GetEventHub();

        private:
            class ServiceLocatorData;
            static ServiceLocatorData* _data;
    };

}

#endif // SPARK_SERVICE_LOCATOR_H
