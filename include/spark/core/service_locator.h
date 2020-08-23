
#ifndef SPARK_SERVICE_LOCATOR_H
#define SPARK_SERVICE_LOCATOR_H

#include <spark/events/event_hub_interface.h>              // IEventHub
#include <spark/utilitybox/logger/logging_hub_interface.h> // ILoggingHub
#include <spark/ecs/entities/entity_manager_interface.h>   // IEntityManager

namespace Spark {

    class ServiceLocator {
        public:
            static void ProvideService(Events::IEventHub* eventHubInterface);
            static void ProvideService(UtilityBox::Logger::ILoggingHub *loggingHubInterface);
            static void ProvideService(ECS::Entities::IEntityManager* entityManagerInterface);

            static Events::IEventHub* GetEventHub();
            static UtilityBox::Logger::ILoggingHub* GetLoggingHub();
            static ECS::Entities::IEntityManager* GetEntityManager();

        private:
            class ServiceLocatorData;
            static ServiceLocatorData* _data;
    };

}

#endif // SPARK_SERVICE_LOCATOR_H
