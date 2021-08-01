
#ifndef SPARK_EVENT_INTERACTABLE_INTERFACE_TPP
#define SPARK_EVENT_INTERACTABLE_INTERFACE_TPP

#include <spark/core/rename.h>
#include <spark/core/service_locator.h>               // ServiceLocator
#include <spark/events/types/event.h>                 // Event
#include <spark/events/event_hub_private_interface.h> // IEventHubPrivate
#include <spark/events/event_listener.h>              // EventListener

namespace Spark::Events {

    template <class Base, class ...EventTypes>
    IEventReceivable<Base, EventTypes...>::IEventReceivable(const std::string& name) : _name(name),
                                                                                    _eventListener(name, CallbackFromMemberFn(this, &IEventReceivable<EventTypes...>::ProcessEvents))
                                                                                    {
        static_assert(sizeof...(EventTypes) > 0, "IEventReceivable must take at least one Event type.");
        dynamic_cast<IEventHubPrivate*>(ServiceLocator::GetEventHub())->AttachEventListener(&_eventListener);
    }

    template <class Base, class ...EventTypes>
    IEventReceivable<Base, EventTypes...>::~IEventReceivable() {
    }

    template <class Base, class ...EventTypes>
    template <class... AdditionalEventTypes>
    void IEventReceivable<Base, EventTypes...>::GetEventOfType(IEvent *baseEvent) {
        PARAMETER_PACK_EXPAND(GetEventOfTypeHelper, AdditionalEventTypes, baseEvent);
    }

    template <class Base, class ...EventTypes>
    template <class EventType>
    void IEventReceivable<Base, EventTypes...>::GetEventOfTypeHelper(IEvent *baseEvent) {
        if (auto derivedEvent = dynamic_cast<EventType*>(baseEvent)) {
            static_cast<Base*>(this)->OnEvent(derivedEvent);
        }
    }

    template <class Base, class ...EventTypes>
    void IEventReceivable<Base, EventTypes...>::ProcessEvents(std::queue<std::shared_ptr<IEvent *>> &eventData) {
        while (!eventData.empty()) {
            std::shared_ptr<IEvent*> eventPtr = eventData.front();
            GetEventOfType<EventTypes...>(*eventPtr);
            eventPtr.reset();
            eventData.pop();
        }
    }

}

#endif // SPARK_EVENT_INTERACTABLE_INTERFACE_TPP
