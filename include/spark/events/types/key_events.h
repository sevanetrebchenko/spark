
#ifndef SPARK_KEY_EVENTS_H
#define SPARK_KEY_EVENTS_H

#include "spark/core/utility.h"
#include "spark/events/types/base_event.h"

namespace Spark {
    namespace Events {

        class KeyPressEvent : public IEvent {
            public:
                REGISTER_EVENT(KeyPressEvent);

                explicit KeyPressEvent(int keyCode);
                NODISCARD int GetKeyCode() const;

            private:
                int keyCode_;
        };


        class KeyReleaseEvent : public IEvent {
            public:
                REGISTER_EVENT(KeyReleaseEvent);

                explicit KeyReleaseEvent(int keyCode);
                NODISCARD int GetKeyCode() const;

            private:
                int keyCode_;
        };

    }
}

#endif // SPARK_KEY_EVENTS_H
