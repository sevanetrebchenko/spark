
#include "spark/events/event_hub.h"
#include "spark/utilitybox/logger/logger.h"

namespace Spark::Events {

    void EventHub::AttachEventListener(IEventListener* toAdd) {
        if (!toAdd) {
            return;
        }

        // Ensure event listener has not already been added.
        for (IEventListener* eventListener : listeners_) {
            if (eventListener == toAdd) {
                LogWarning("Adding instance of event listener that has already been registered with the EventHub. Listener will not be added.");
                return;
            }
        }

        listeners_.emplace_back(toAdd);
    }

    void EventHub::DetachEventListener(IEventListener* toRemove) {
        if (!toRemove) {
            return;
        }

        for (auto listenerIter = listeners_.begin(); listenerIter != listeners_.end(); ++listenerIter) {
            if (*listenerIter == toRemove) {
                listeners_.erase(listenerIter);
                return;
            }
        }
    }

    void EventHub::Dispatch(const EventHandle& event) {
        for (IEventListener* eventListener : listeners_) {
            eventListener->OnEventReceived(event);
        }
    }

    void Spark::Events::EventHub::OnUpdate(float dt) {
        for (IEventListener* eventListener : listeners_) {
            eventListener->OnUpdate(dt); // Updated in the order listeners are inserted.
        }
    }

}
