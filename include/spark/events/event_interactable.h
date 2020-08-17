
#ifndef SPARK_EVENT_INTERACTABLE_H
#define SPARK_EVENT_INTERACTABLE_H

#include <events/event_listener.h>
#include <events/application_events.h>
#include <core/service_locator.h>
#include <utilitybox/tools/utility_functions.h>

#define PARAMETER_PACK_EXPAND(function, args, ...) ((void)function<args>(__VA_ARGS__), ...);

namespace Spark {
    namespace Events {

        template<class First, class ...Rest>
        struct RequireTypes : RequireTypes<First>, RequireTypes<Rest...> {
            using RequireTypes<First>::OnEvent;
        };

        template<class First>
        struct RequireTypes<First> {
            virtual void OnEvent(First* param) = 0;
        };

        template<class ChildClass, class ...EventTypes>
        class IEventReceivable : public RequireTypes<EventTypes...> {
            public:
                void OnUpdate();

                template <class EventType>
                bool CheckEventPointer(EventType* eventPointer);

            protected:
                IEventReceivable(); // Make this class abstract.

            private:
                void ProcessEvents(std::queue<std::shared_ptr<Event*>>& eventData);

                template<class ...AdditionalEventTypes>
                void GetEventOfType(Event* baseEvent);

                template<class CurrentEventType>
                void GetEventOfTypeHelper(Event* baseEvent);

                Events::EventListener<EventTypes...> _eventListener { CallbackFromMemberFn(this, &IEventReceivable::ProcessEvents) };
                Event* _currentEvent;
        };

        template<class ChildClass, class ...EventTypes>
        void IEventReceivable<ChildClass, EventTypes...>::ProcessEvents(std::queue<std::shared_ptr<Event *>> &eventData) {
            while (!eventData.empty()) {
                std::shared_ptr<Event*> eventPtr = eventData.front();
                GetEventOfType<EventTypes...>(*eventPtr);
                eventPtr.reset();
                eventData.pop();
            }
        }

        template<class ChildClass, class ...EventTypes>
        template<class... AdditionalEventTypes>
        void IEventReceivable<ChildClass, EventTypes...>::GetEventOfType(Event* baseEvent) {
            PARAMETER_PACK_EXPAND(GetEventOfTypeHelper, AdditionalEventTypes, baseEvent);
        }

        template<class ChildClass, class ...EventTypes>
        template<class CurrentEventType>
        void IEventReceivable<ChildClass, EventTypes...>::GetEventOfTypeHelper(Event *baseEvent) {
            if (auto derivedEvent = dynamic_cast<CurrentEventType*>(baseEvent)) {
                _currentEvent = baseEvent;
                static_cast<ChildClass*>(this)->OnEvent(derivedEvent);
                _currentEvent = nullptr;
            }
        }

        template<class ChildClass, class ...EventTypes>
        IEventReceivable<ChildClass, EventTypes...>::IEventReceivable() {
            ServiceLocator::GetEventHub()->AttachEventListener(&_eventListener);
        }

        template<class ChildClass, class ...EventTypes>
        void IEventReceivable<ChildClass, EventTypes...>::OnUpdate() {
            _eventListener.OnUpdate();
        }

        template<class ChildClass, class... EventTypes>
        template<class EventType>
        bool IEventReceivable<ChildClass, EventTypes...>::CheckEventPointer(EventType *eventPointer) {
            return dynamic_cast<EventType*>(_currentEvent) == eventPointer;
        }

    }
}

#endif //SPARK_EVENT_INTERACTABLE_H
