
#ifndef SPARK_ON_EVENT_FUNCTION_GENERATOR_H
#define SPARK_ON_EVENT_FUNCTION_GENERATOR_H

namespace Spark {
    namespace Events {

        template <class EventType, class ...AdditionalEventTypes>
        struct RequireOnEventFunctionForTypes : RequireOnEventFunctionForTypes<EventType>, RequireOnEventFunctionForTypes<AdditionalEventTypes...> {
            using RequireOnEventFunctionForTypes<EventType>::OnEvent;
        };

        template <class EventType>
        struct RequireOnEventFunctionForTypes<EventType> {
            /**
             * Required overload generated. Takes in an event pointer of the provided EventType.
             * @param eventPointer - Event pointer of the provided EventType.
             */
            virtual void OnEvent(EventType* eventPointer) = 0;
        };

    }
}

#endif //SPARK_ON_EVENT_FUNCTION_GENERATOR_H
