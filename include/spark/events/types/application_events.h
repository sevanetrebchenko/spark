
#ifndef SPARK_APPLICATION_EVENTS_H
#define SPARK_APPLICATION_EVENTS_H

#include <spark/core/rename.h>
#include <spark/events/types/event.h> // Event, EventType

namespace Spark {
    namespace Events {

        class WindowResizeEvent : public IEvent {
            public:
                static inline constexpr EventType StaticEventType = EventType::WindowResized;
                WindowResizeEvent(int width, int height);

                NODISCARD std::string ToString() const override;
                NODISCARD int GetWidth() const;
                NODISCARD int GetHeight() const;

            private:
                int _width;
                int _height;
        };

        class WindowCloseEvent : public IEvent {
            public:
                static inline constexpr EventType StaticEventType = EventType::WindowCloseRequested;
                WindowCloseEvent();

                NODISCARD std::string ToString() const override;
        };

        class WindowMinimizedEvent : public IEvent {
            public:
                static inline constexpr EventType StaticEventType = EventType::WindowMinimized;
                explicit WindowMinimizedEvent(bool minimized);

                NODISCARD std::string ToString() const override;
                NODISCARD bool GetMinimized() const;
            private:
                bool _minimized;
        };

    }
}

#endif // SPARK_APPLICATION_EVENTS_H
