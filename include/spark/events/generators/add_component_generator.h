

#ifndef SPARK_GENERATE_ADD_COMPONENT_H
#define SPARK_GENERATE_ADD_COMPONENT_H

#include <spark/events/types/ecs_events.h> // AddComponentEvent

namespace Spark {
    namespace Events {

        template <class EventType, class ...AdditionalEventTypes>
        struct GenerateAddComponentsForTypes : GenerateAddComponentsForTypes<EventType>, GenerateAddComponentsForTypes<AdditionalEventTypes...> {
        };

        template <class EventType>
        struct GenerateAddComponentsForTypes<EventType> {
            using Type = AddComponentEvent<EventType>;
        };

    }
}

#endif // SPARK_GENERATE_ADD_COMPONENT_H
