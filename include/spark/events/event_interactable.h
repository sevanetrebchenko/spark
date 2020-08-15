
#ifndef SPARK_EVENT_INTERACTABLE_H
#define SPARK_EVENT_INTERACTABLE_H

#include <events/event_listener.h>
#include <core/service_locator.h>
#include <utilitybox/tools/utility_functions.h>

namespace Spark {
    namespace Events {

        /**
         * Classes that derive from the IEventReceivable interface register themselves as classes that can receive events.
         * Types of events that can be received are dictated by the template parameters to the interface.
         * @tparam EventTypes - Desired event types to track with this interface.
         */
        template <class ...EventTypes>
        class IEventReceivable {
            public:
                /**
                 * Registers the 'ProcessEvents' function with the underlying EventListener.
                 */
                IEventReceivable();
                void OnUpdate();
                virtual void ProcessEvents(std::queue<std::shared_ptr<Event*>>& eventData) = 0;

            private:
                Events::EventListener<EventTypes...> _eventListener { CallbackFromMemberFn(this, &IEventReceivable::ProcessEvents) };
        };

        template<class... EventTypes>
        IEventReceivable<EventTypes...>::IEventReceivable() {
            ServiceLocator::GetEventHub()->AttachEventListener(&_eventListener);
        }

        template<class... EventTypes>
        void IEventReceivable<EventTypes...>::OnUpdate() {
            _eventListener.OnUpdate();
        }

    }
}

#endif //SPARK_EVENT_INTERACTABLE_H
