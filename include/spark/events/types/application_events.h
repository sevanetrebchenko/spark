
#ifndef SPARK_APPLICATION_EVENTS_H
#define SPARK_APPLICATION_EVENTS_H

#include <spark_pch.h>                             // std::string
#include <spark/events/types/event.h>              // Event, EventType
#include <spark/utilitybox/tools/global_defines.h> // _NODISCARD_

namespace Spark {
    namespace Events {

        class WindowResizeEvent : public Event {
            public:
                static inline constexpr EventType StaticEventType = EventType::WindowResized;
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
                static inline constexpr EventType StaticEventType = EventType::WindowCloseRequested;
                WindowCloseEvent();

                _NODISCARD_ std::string ToString() const override;
        };

    }
}

#endif // SPARK_APPLICATION_EVENTS_H
