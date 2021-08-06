
#ifndef SPARK_EVENT_INTERACTABLE_INTERFACE_TPP
#define SPARK_EVENT_INTERACTABLE_INTERFACE_TPP

#include <spark/core/rename.h>
#include <spark/events/types/event.h>                 // Event
#include <spark/events/event_listener.h>              // EventListener
#include <spark/events/event_hub.h>

namespace Spark::Events {

    template <class CRTP, class ...EventTypes>
    IEventReceivable<CRTP, EventTypes...>::IEventReceivable(const std::string& name) : _name(name),
                                                                                    _eventListener(name, CallbackFromMemberFn(this, &IEventReceivable<EventTypes...>::ProcessEvents))
                                                                                    {
        static_assert(sizeof...(EventTypes) > 0, "IEventReceivable must take at least one Event type.");
        Singleton<EventHub>::GetInstance()->AttachEventListener(&_eventListener);
    }

    template <class CRTP, class ...EventTypes>
    IEventReceivable<CRTP, EventTypes...>::~IEventReceivable() {
    }

    template <class CRTP, class ...EventTypes>
    template <class... AdditionalEventTypes>
    void IEventReceivable<CRTP, EventTypes...>::GetEventOfType(IEvent *baseEvent) {
        PARAMETER_PACK_EXPAND(GetEventOfTypeHelper, AdditionalEventTypes, baseEvent);
    }

    template <class CRTP, class ...EventTypes>
    template <class EventType>
    void IEventReceivable<CRTP, EventTypes...>::GetEventOfTypeHelper(IEvent *baseEvent) {
        if (auto derivedEvent = dynamic_cast<EventType*>(baseEvent)) {
            static_cast<CRTP*>(this)->OnEvent(derivedEvent);
        }
    }

    template <class CRTP, class ...EventTypes>
    void IEventReceivable<CRTP, EventTypes...>::ProcessEvents(std::queue<std::shared_ptr<IEvent *>> &eventData) {
        while (!eventData.empty()) {
            std::shared_ptr<IEvent*> eventPtr = eventData.front();
            GetEventOfType<EventTypes...>(*eventPtr);
            eventPtr.reset();
            eventData.pop();
        }
    }

}

#endif // SPARK_EVENT_INTERACTABLE_INTERFACE_TPP
