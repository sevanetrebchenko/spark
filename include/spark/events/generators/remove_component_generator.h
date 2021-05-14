
#ifndef SPARK_REMOVE_COMPONENT_GENERATOR_H
#define SPARK_REMOVE_COMPONENT_GENERATOR_H

#include <spark/events/types/ecs_events.h> // RemoveComponentEvent

namespace Spark {
    namespace Events {

        template <class EventType, class ...AdditionalEventTypes>
        struct GenerateRemoveComponentsForTypes : RemoveComponentEvent<EventType>, GenerateRemoveComponentsForTypes<AdditionalEventTypes...> {
        };

        template <class EventType>
        struct GenerateRemoveComponentsForTypes<EventType> {
            using Type = RemoveComponentEvent<EventType>;
        };

    }
}

#endif // SPARK_REMOVE_COMPONENT_GENERATOR_H
