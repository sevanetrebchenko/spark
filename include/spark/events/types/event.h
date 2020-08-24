
#ifndef SPARK_EVENT_H
#define SPARK_EVENT_H

#include <spark_pch.h>                             // std::string
#include <spark/utilitybox/tools/global_defines.h> // _NODISCARD_

namespace Spark {
    namespace Events {

        enum class EventType {
            None = 0,
            WindowCloseRequested, WindowResized, WindowMinimized,
            KeyPressed, KeyReleased,
            MouseButtonPressed, MouseButtonReleased, MouseScrolled, MouseMoved,
            EntityComponentAdd, EntityComponentRemove,
        };

        enum EventCategory {
            None = 0,
            EventCategoryApplication = BIT_SHIFT(0u),
            EventCategoryInput = BIT_SHIFT(1u),
            EventCategoryECS = BIT_SHIFT(2u),
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
                virtual ~Event() {
                    std::cout << "event destructor" << std::endl;
                }

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

                _NODISCARD_ static const char * ConvertEventTypeToString(const EventType& eventType);

            protected:
                const EventType _eventType;
                const EventCategory _eventCategory;
        };

    }
}

#endif // SPARK_EVENT_H
