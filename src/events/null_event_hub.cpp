
#include <events/null_event_hub.h>

namespace Spark::Events {

    void NullEventHub::AttachEventListener(IEventListener*) {
        // Do nothing.
    }

    void NullEventHub::DetachEventListener(IEventListener*) {
        // Do nothing.
    }

    void NullEventHub::Dispatch(Event*) {
        // Do nothing.
    }

    void NullEventHub::OnUpdate() {
        // Do nothing.
    }
}
