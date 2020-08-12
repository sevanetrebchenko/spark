
#ifndef SPARK_EVENT_H
#define SPARK_EVENT_H

namespace Spark {
    namespace Events {

        enum class EventType {
            None = 0,
            WindowCloseRequested, WindowResized
        };

        enum EventCategory {
            None = 0,
            EventCategoryApplication = (1u << 0u),
        };

        class Event {
            public:
                Event(EventType eventType, EventCategory eventCategory);
                virtual ~Event() = default;
                virtual EventType GetEventType();

            private:
                EventType _eventType;
                EventCategory _eventCategory;
        };

    }
}

#endif //SPARK_EVENT_H
