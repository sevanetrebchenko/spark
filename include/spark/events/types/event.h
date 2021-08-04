
#ifndef SPARK_EVENT_H
#define SPARK_EVENT_H

#include <spark/core/rename.h>

namespace Spark {
    namespace Events {

        enum class EventType {
            None = 0,
            WindowCloseRequested, WindowResized, WindowMinimized,
            KeyPressed, KeyReleased,
            MouseButtonPressed, MouseButtonReleased, MouseScrolled, MouseMoved,
            EntityCreate, EntityDestroy, EntityComponentAdd, EntityComponentRemove,
            SystemRefreshObjectComponentList,
        };

        enum class EventCategory {
            None = 0,
            EventCategoryApplication = BIT_SHIFT(0u),
            EventCategoryInput = BIT_SHIFT(1u),
            EventCategoryECS = BIT_SHIFT(2u),
        };

        // Abstract event class type - should not be instantiated, strictly derived from.
        class IEvent {
            public:
                IEvent(EventType eventType, EventCategory eventCategory);
                virtual ~IEvent() = 0;

                NODISCARD virtual std::string ToString() const = 0;
                NODISCARD const EventType& GetEventType() const;
                NODISCARD const EventCategory& GetEventCategory() const;
                NODISCARD bool IsInEventCategory(EventCategory eventCategory) const;
                NODISCARD static std::string ConvertEventTypeToString(const EventType& eventType);

            protected:
                const EventType _eventType;
                const EventCategory _eventCategory;
        };

    }
}

#endif // SPARK_EVENT_H
