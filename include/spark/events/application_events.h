
#ifndef SPARK_APPLICATION_EVENTS_H
#define SPARK_APPLICATION_EVENTS_H

#include <events/event.h>
#include <utilitybox/tools/global_defines.h>

namespace Spark {
    namespace Events {

        class WindowResizeEvent : public Event {
            public:
                static inline constexpr EventType GetStaticEventType() { return EventType::WindowResized; }
                WindowResizeEvent(unsigned width, unsigned height);

                _NODISCARD_ std::string ToString() const override;
                _NODISCARD_ unsigned GetWidth() const;
                _NODISCARD_ unsigned GetHeight() const;

            private:
                unsigned _width;
                unsigned _height;
        };

        class WindowCloseEvent : public Event {
            public:
                static inline constexpr EventType GetStaticEventType() { return EventType::WindowCloseRequested; }
                _NODISCARD_ std::string ToString() const override;
                WindowCloseEvent();
        };

    }
}

#endif // SPARK_APPLICATION_EVENTS_H
