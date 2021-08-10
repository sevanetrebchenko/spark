
#include "spark/events/types/key_events.h"

namespace Spark::Events {

    // KeyPressEvent.
    KeyPressEvent::KeyPressEvent(int keyCode) : keyCode_(keyCode) {
    }

    int KeyPressEvent::GetKeyCode() const {
        return keyCode_;
    }


    // KeyReleaseEvent.
    KeyReleaseEvent::KeyReleaseEvent(int keyCode) : keyCode_(keyCode) {
    }

    int KeyReleaseEvent::GetKeyCode() const {
        return keyCode_;
    }

}
