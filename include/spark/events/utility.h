
#ifndef SPARK_EVENTS_UTILITY_H
#define SPARK_EVENTS_UTILITY_H

#include <spark/core/utility.h>
#include <spark/events/types/ecs_events.h>

namespace Spark {
    namespace Internal {

        // OnEvent.
        template <typename... EventTypes>
        struct RequireOnEventForTypes {
            static_assert(sizeof...(EventTypes) > 0, "Invalid number template arguments.");
        };

        template <typename EventType, typename... AdditionalEventTypes>
        struct RequireOnEventForTypes<EventType, AdditionalEventTypes...> : RequireOnEventForTypes<EventType>, RequireOnEventForTypes<AdditionalEventTypes...> {
            using RequireOnEventForTypes<EventType>::OnEvent;
        };

        template <typename EventType>
        struct RequireOnEventForTypes<EventType> {
            virtual void OnEvent(EventType* eventPointer) = 0;
        };

    }
}

// MACRO FOR DEFINING PURE VIRTUAL OnEvent FUNCTION FOR DERIVED CLASSES TO IMPLEMENT.
#define REGISTER_ON_EVENT(EVENT_TYPES...) Spark::Internal::RequireUniqueTypes<EVENT_TYPES>, Spark::Internal::RequireOnEventForTypes<EVENT_TYPES>

// MACRO FOR REGISTERING CLASSES TO RECEIVE EVENTS OF GIVEN TYPES.
#define REGISTER_EVENTS(BASE_CLASS, ...) Spark::Events::IEventReceivable<BASE_CLASS, __VA_ARGS__>

#endif //SPARK_EVENTS_UTILITY_H
