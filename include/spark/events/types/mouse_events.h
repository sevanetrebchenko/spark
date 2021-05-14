
#ifndef SPARK_MOUSE_EVENTS_H
#define SPARK_MOUSE_EVENTS_H

#include <spark/core/core.h>
#include <spark/events/types/event.h> // Event, EventType

namespace Spark {
    namespace Events {

        class MouseButtonPressedEvent : public Event {
            public:
                static inline constexpr EventType StaticEventType = EventType::MouseButtonPressed;
                explicit MouseButtonPressedEvent(int mouseButtonCode);

                _NODISCARD_ std::string ToString() const override;
                _NODISCARD_ int GetMouseButtonCode() const;

            private:
                int _mouseButtonCode;
        };

        class MouseButtonReleasedEvent : public Event {
            public:
                static inline constexpr EventType StaticEventType = EventType::MouseButtonReleased;
                explicit MouseButtonReleasedEvent(int mouseButtonCode);

                _NODISCARD_ std::string ToString() const override;
                _NODISCARD_ int GetMouseButtonCode() const;

            private:
                int _mouseButtonCode;
        };

        class MouseScrolledEvent : public Event {
            public:
                static inline constexpr EventType StaticEventType = EventType::MouseScrolled;
                explicit MouseScrolledEvent(double scrollX, double scrollY);

                _NODISCARD_ std::string ToString() const override;
                _NODISCARD_ double GetScrollX() const;
                _NODISCARD_ double GetScrollY() const;

            private:
                double _scrollX, _scrollY;
        };

        class MouseMovedEvent : public Event {
            public:
                static inline constexpr EventType StaticEventType = EventType::MouseMoved;
                explicit MouseMovedEvent(int mouseX, int mouseY);

                _NODISCARD_ std::string ToString() const override;
                _NODISCARD_ int GetMouseX() const;
                _NODISCARD_ int GetMouseY() const;

            private:
                int _mouseX, _mouseY;
        };

    }
}

#endif // SPARK_MOUSE_EVENTS_H
