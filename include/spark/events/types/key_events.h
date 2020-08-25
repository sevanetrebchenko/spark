
#ifndef SPARK_KEY_EVENTS_H
#define SPARK_KEY_EVENTS_H

#include <spark_pch.h>                             // std::string
#include <spark/events/types/event.h>              // Event, EventType
#include <utilitybox/tools/global_defines.h> // _NODISCARD_

namespace Spark {
    namespace Events {

        class KeyPressEvent : public Event {
            public:
                static inline constexpr EventType StaticEventType = EventType::KeyPressed;
                explicit KeyPressEvent(int keyCode);

                _NODISCARD_ std::string ToString() const override;
                _NODISCARD_ int GetKeyCode() const;

            private:
                int _keyCode;
        };

        class KeyReleaseEvent : public Event {
            public:
                static inline constexpr EventType StaticEventType = EventType::KeyReleased;
                explicit KeyReleaseEvent(int keyCode);

                _NODISCARD_ std::string ToString() const override;
                _NODISCARD_ int GetKeyCode() const;

            private:
                int _keyCode;
        };

    }
}

#endif // SPARK_KEY_EVENTS_H
