
#ifndef SPARK_EVENTS_UTILITY_H
#define SPARK_EVENTS_UTILITY_H

#include "spark/core/utility.h"

namespace Spark {
    namespace Events {

        class IEvent;

        typedef std::uint32_t EventTypeID;
        typedef std::shared_ptr<const IEvent> EventHandle;

        namespace Internal {

            // OnEvent.
            template <typename... EventTypes>
            struct RequireOnEventForTypes {
                static_assert(sizeof...(EventTypes) > 0, "Invalid number template arguments provided to RequireOnEventForTypes.");
            };

            template <typename EventType, typename... AdditionalEventTypes>
            struct RequireOnEventForTypes<EventType, AdditionalEventTypes...> : RequireOnEventForTypes<EventType>, RequireOnEventForTypes<AdditionalEventTypes...> {
                using RequireOnEventForTypes<EventType>::OnEvent;
            };

            template <typename EventType>
            struct RequireOnEventForTypes<EventType> {
                virtual void OnEvent(const EventType* eventPointer) = 0;
            };

        }

    }
}

#define REGISTER_EVENT(ClassName)                                              \
public:                                                                        \
    static constexpr Spark::Events::EventTypeID ID = STRINGHASH(#ClassName);   \
    static constexpr const char* Name = #ClassName;                            \
    NODISCARD Spark::Events::EventTypeID GetID() const override { return ID; }

#endif //SPARK_EVENTS_UTILITY_H
