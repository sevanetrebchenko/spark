
#ifndef SPARK_APPLICATION_EVENTS_H
#define SPARK_APPLICATION_EVENTS_H

#include "spark/utility.h"
#include "spark/events/types/event.h"

namespace Spark {
    namespace Events {

        class WindowResizeEvent : public IEvent {
            public:
                REGISTER_EVENT(WindowResizeEvent);

                WindowResizeEvent(int previousWidth, int previousHeight, int width, int height);

                NODISCARD int GetWidth() const;
                NODISCARD int GetHeight() const;

                NODISCARD int GetPreviousWidth() const;
                NODISCARD int GetPreviousHeight() const;

            private:
                int width_;
                int previousWidth_;
                int height_;
                int previousHeight_;
        };


        class WindowCloseEvent : public IEvent {
            public:
                REGISTER_EVENT(WindowCloseEvent);
        };


        class WindowMinimizedEvent : public IEvent {
            public:
                REGISTER_EVENT(WindowMinimizedEvent);

                explicit WindowMinimizedEvent(bool minimized);
                NODISCARD bool GetMinimized() const;

            private:
                bool minimized_;
        };

    }
}

#endif // SPARK_APPLICATION_EVENTS_H
