
#ifndef SPARK_MOUSE_EVENTS_H
#define SPARK_MOUSE_EVENTS_H

#include <spark/core/rename.h>
#include <spark/events/types/event.h> // Event, EventType

namespace Spark {
    namespace Events {

        class MouseButtonPressedEvent : public Event {
            public:
                static inline constexpr EventType StaticEventType = EventType::MouseButtonPressed;
                explicit MouseButtonPressedEvent(int mouseButtonCode);

                NODISCARD std::string ToString() const override;
                NODISCARD int GetMouseButtonCode() const;

            private:
                int _mouseButtonCode;
        };

        class MouseButtonReleasedEvent : public Event {
            public:
                static inline constexpr EventType StaticEventType = EventType::MouseButtonReleased;
                explicit MouseButtonReleasedEvent(int mouseButtonCode);

                NODISCARD std::string ToString() const override;
                NODISCARD int GetMouseButtonCode() const;

            private:
                int _mouseButtonCode;
        };

        class MouseScrolledEvent : public Event {
            public:
                static inline constexpr EventType StaticEventType = EventType::MouseScrolled;
                explicit MouseScrolledEvent(double scrollX, double scrollY);

                NODISCARD std::string ToString() const override;
                NODISCARD double GetScrollX() const;
                NODISCARD double GetScrollY() const;

            private:
                double _scrollX, _scrollY;
        };

        class MouseMovedEvent : public Event {
            public:
                static inline constexpr EventType StaticEventType = EventType::MouseMoved;
                explicit MouseMovedEvent(int mouseX, int mouseY);

                NODISCARD std::string ToString() const override;
                NODISCARD int GetMouseX() const;
                NODISCARD int GetMouseY() const;

            private:
                int _mouseX, _mouseY;
        };

    }
}

#endif // SPARK_MOUSE_EVENTS_H
