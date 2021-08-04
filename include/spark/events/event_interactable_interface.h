
#ifndef SPARK_EVENT_INTERACTABLE_INTERFACE_H
#define SPARK_EVENT_INTERACTABLE_INTERFACE_H

#include <spark/events/event_listener.h>
#include "utility.h"

namespace Spark {
    namespace Events {

        template <class Base, class... EventTypes>
        class IEventReceivable : REGISTER_ON_EVENT(EventTypes...) {
            public:
                ~IEventReceivable();

            protected:
                explicit IEventReceivable(const std::string& name = "Not provided"); // Make this class abstract.

            private:
                void ProcessEvents(std::queue<std::shared_ptr<IEvent*>>& eventData);

                template<class ...AdditionalEventTypes>
                void GetEventOfType(IEvent* baseEvent);

                template<class CurrentEventType>
                void GetEventOfTypeHelper(IEvent* baseEvent);

                std::string _name;
                EventListener<EventTypes...> _eventListener;
        };

    }
}

#include <spark/events/event_interactable_interface.tpp> // Template function definitions.

#endif // SPARK_EVENT_INTERACTABLE_INTERFACE_H
