
#ifndef SPARK_EVENT_INTERACTABLE_INTERFACE_TPP
#define SPARK_EVENT_INTERACTABLE_INTERFACE_TPP

#include <spark_pch.h>                          // std::queue, std::shared_ptr
#include <spark/core/service_locator.h>         // ServiceLocator
#include <spark/events/types/event.h>           // Event
#include <events/event_hub_private_interface.h> // IEventHubPrivate
#include <utilitybox/tools/utility_functions.h> // PARAMETER_PACK_EXPAND, CallbackFromMemberFn
#include <events/event_listener.h>              // EventListener

namespace Spark::Events {

    template<class ChildClass, class ...EventTypes>
    class IEventReceivable<ChildClass, EventTypes...>::IEventReceivableData {
        public:
            IEventReceivableData(const char* name, IEventReceivable<ChildClass, EventTypes...>* parent);
            ~IEventReceivableData();

            template <class EventType>
            bool CheckEventPointer(EventType* eventPointer);

        private:
            void ProcessEvents(std::queue<std::shared_ptr<Event*>>& eventData);

            template<class ...AdditionalEventTypes>
            void GetEventOfType(Event* baseEvent);

            template<class CurrentEventType>
            void GetEventOfTypeHelper(Event* baseEvent);

            const char* _name;
            IEventReceivable<ChildClass, EventTypes...>* _parent;
            Events::EventListener<EventTypes...> _eventListener { _name, CallbackFromMemberFn(this, &IEventReceivableData::ProcessEvents) };
            Event* _currentEvent;
    };

    template<class ChildClass, class... EventTypes>
    IEventReceivable<ChildClass, EventTypes...>::IEventReceivableData::IEventReceivableData(const char* name, IEventReceivable<ChildClass, EventTypes...>* parent) : _name(name), _parent(parent), _currentEvent(nullptr) {
        dynamic_cast<IEventHubPrivate*>(ServiceLocator::GetEventHub())->AttachEventListener(&_eventListener);
    }

    template<class ChildClass, class... EventTypes>
    void IEventReceivable<ChildClass, EventTypes...>::IEventReceivableData::ProcessEvents(std::queue<std::shared_ptr<Event *>> &eventData) {
        while (!eventData.empty()) {
            std::shared_ptr<Event*> eventPtr = eventData.front();
            GetEventOfType<EventTypes...>(*eventPtr);
            eventPtr.reset();
            eventData.pop();
        }
    }

    template<class ChildClass, class... EventTypes>
    template<class... AdditionalEventTypes>
    void IEventReceivable<ChildClass, EventTypes...>::IEventReceivableData::GetEventOfType(Event *baseEvent) {
        PARAMETER_PACK_EXPAND(GetEventOfTypeHelper, AdditionalEventTypes, baseEvent);
    }

    template<class ChildClass, class... EventTypes>
    template<class CurrentEventType>
    void IEventReceivable<ChildClass, EventTypes...>::IEventReceivableData::GetEventOfTypeHelper(Event *baseEvent) {
        if (auto derivedEvent = dynamic_cast<CurrentEventType*>(baseEvent)) {
            _currentEvent = baseEvent;
            static_cast<ChildClass*>(_parent)->OnEvent(derivedEvent);
            _currentEvent = nullptr;
        }
    }

    template<class ChildClass, class... EventTypes>
    template<class EventType>
    bool IEventReceivable<ChildClass, EventTypes...>::IEventReceivableData::CheckEventPointer(EventType *eventPointer) {
        return dynamic_cast<EventType*>(_currentEvent) == eventPointer;
    }

    template<class ChildClass, class... EventTypes>
    IEventReceivable<ChildClass, EventTypes...>::IEventReceivableData::~IEventReceivableData() {
        _currentEvent = nullptr;
        _parent = nullptr;
    }

    template<class ChildClass, class... EventTypes>
    template<class EventType>
    bool IEventReceivable<ChildClass, EventTypes...>::CheckEventPointer(EventType *eventPointer) {
        return _data->template CheckEventPointer<EventType>(eventPointer);
    }

    template<class ChildClass, class... EventTypes>
    IEventReceivable<ChildClass, EventTypes...>::IEventReceivable(const char* name) : _data(new IEventReceivableData(name, this)) {
        // Nothing to do here.
    }
}

#endif // SPARK_EVENT_INTERACTABLE_INTERFACE_TPP
