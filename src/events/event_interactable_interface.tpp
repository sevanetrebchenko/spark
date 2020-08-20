
#ifndef SPARK_EVENT_INTERACTABLE_INTERFACE_TPP
#define SPARK_EVENT_INTERACTABLE_INTERFACE_TPP

#include <events/event_listener.h>
#include <events/application_events.h>
#include <core/service_locator.h>
#include <utilitybox/tools/utility_functions.h>

namespace Spark::Events {

    template<class ChildClass, class ...EventTypes>
    class IEventReceivable<ChildClass, EventTypes...>::IEventReceivableData {
        public:
            explicit IEventReceivableData(IEventReceivable<ChildClass, EventTypes...>* parent);
            ~IEventReceivableData();

            template <class EventType>
            bool CheckEventPointer(EventType* eventPointer);

        private:
            void ProcessEvents(std::queue<std::shared_ptr<Event*>>& eventData);

            template<class ...AdditionalEventTypes>
            void GetEventOfType(Event* baseEvent);

            template<class CurrentEventType>
            void GetEventOfTypeHelper(Event* baseEvent);

            IEventReceivable<ChildClass, EventTypes...>* _parent;
            Events::EventListener<EventTypes...> _eventListener { CallbackFromMemberFn(this, &IEventReceivableData::ProcessEvents) };
            Event* _currentEvent;
    };

    template<class ChildClass, class... EventTypes>
    IEventReceivable<ChildClass, EventTypes...>::IEventReceivableData::IEventReceivableData(IEventReceivable<ChildClass, EventTypes...>* parent) : _parent(parent), _currentEvent(nullptr) {
        ServiceLocator::GetEventHub()->AttachEventListener(&_eventListener);
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
    IEventReceivable<ChildClass, EventTypes...>::IEventReceivable() : _data(new IEventReceivableData(this)) {
        // Nothing to do here.
    }
}

#endif // SPARK_EVENT_INTERACTABLE_INTERFACE_TPP
