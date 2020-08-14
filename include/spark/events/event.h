
#ifndef SPARK_EVENT_H
#define SPARK_EVENT_H

#include <utilitybox/tools/global_defines.h> // _NODISCARD_
#include <spark_pch.h>                       // std::string

namespace Spark {
    namespace Events {

        enum class EventType {
            None = 0,
            WindowCloseRequested, WindowResized,
            KeyPressed, KeyReleased,
            MouseButtonPressed, MouseButtonReleased, MouseScrolled, MouseMoved
        };

        enum EventCategory {
            None = 0,
            EventCategoryApplication = BIT_SHIFT(0u),
            EventCategoryInput = BIT_SHIFT(1u)
        };

        // Abstract event class type - should not be instantiated, strictly derived from.
        class Event {
            public:
                /**
                 * Construct an event given the event type and category the event falls under.
                 * @param eventType     - Type of event.
                 * @param eventCategory - Category of event.
                 */
                Event(EventType eventType, EventCategory eventCategory);

                /**
                 * Destructor.
                 */
                virtual ~Event() = default;

                /**
                 * Convert the event to a human-readable string.
                 * @return Human-readable representation of the event.
                 */
                _NODISCARD_ virtual std::string ToString() const = 0;

                /**
                 * Get the type of this event.
                 * @return Type of this event.
                 */
                _NODISCARD_ const EventType& GetEventType() const;

                /**
                 * Get the category of this event.
                 * @return Category of this event.
                 */
                _NODISCARD_ const EventCategory& GetEventCategory() const;

                /**
                 * Query whether the event is within a certain category of events.
                 * @param eventCategory - Event category to query against.
                 * @return True  - the event falls under the given event category.
                 *         False - the event does not fall under the given event category.
                 */
                _NODISCARD_ bool IsInEventCategory(EventCategory eventCategory) const;

            protected:
                const EventType _eventType;
                const EventCategory _eventCategory;
        };

    }
}

#endif //SPARK_EVENT_H
