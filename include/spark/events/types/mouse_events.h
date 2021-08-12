
#ifndef SPARK_MOUSE_EVENTS_H
#define SPARK_MOUSE_EVENTS_H

#include "spark/utility.h"
#include "spark/events/types/base_event.h"

namespace Spark {
    namespace Events {

        class MouseButtonPressedEvent : public IEvent {
            public:
                REGISTER_EVENT(MouseButtonPressedEvent);

                explicit MouseButtonPressedEvent(int mouseButtonCode);
                NODISCARD int GetMouseButtonCode() const;

            private:
                int mouseButtonCode_;
        };


        class MouseButtonReleasedEvent : public IEvent {
            public:
                REGISTER_EVENT(MouseButtonReleasedEvent);

                explicit MouseButtonReleasedEvent(int mouseButtonCode);
                NODISCARD int GetMouseButtonCode() const;

            private:
                int mouseButtonCode_;
        };


        class MouseScrolledEvent : public IEvent {
            public:
                REGISTER_EVENT(MouseScrolledEvent);

                explicit MouseScrolledEvent(double scrollX, double scrollY);
                NODISCARD double GetScrollX() const;
                NODISCARD double GetScrollY() const;

            private:
                double scrollX_;
                double scrollY_;
        };


        class MouseMovedEvent : public IEvent {
            public:
                REGISTER_EVENT(MouseMovedEvent);

                explicit MouseMovedEvent(int mouseX, int mouseY);
                NODISCARD int GetMouseX() const;
                NODISCARD int GetMouseY() const;

            private:
                int mouseX_;
                int mouseY_;
        };

    }
}

#endif // SPARK_MOUSE_EVENTS_H
