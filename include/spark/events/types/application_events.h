
#ifndef SPARK_APPLICATION_EVENTS_H
#define SPARK_APPLICATION_EVENTS_H

#include <spark/core/core.h>
#include <spark/events/types/event.h> // Event, EventType

namespace Spark {
    namespace Events {

        class WindowResizeEvent : public Event {
            public:
                static inline constexpr EventType StaticEventType = EventType::WindowResized;
                WindowResizeEvent(int width, int height);

                _NODISCARD_ std::string ToString() const override;
                _NODISCARD_ int GetWidth() const;
                _NODISCARD_ int GetHeight() const;

            private:
                int _width;
                int _height;
        };

        class WindowCloseEvent : public Event {
            public:
                static inline constexpr EventType StaticEventType = EventType::WindowCloseRequested;
                WindowCloseEvent();

                _NODISCARD_ std::string ToString() const override;
        };

        class WindowMinimizedEvent : public Event {
            public:
                static inline constexpr EventType StaticEventType = EventType::WindowMinimized;
                explicit WindowMinimizedEvent(bool minimized);

                _NODISCARD_ std::string ToString() const override;
                _NODISCARD_ bool GetMinimized() const;
            private:
                bool _minimized;
        };

    }
}

#endif // SPARK_APPLICATION_EVENTS_H
