
#ifndef SPARK_EVENT_INTERACTABLE_H
#define SPARK_EVENT_INTERACTABLE_H

#include <events/event_listener.h>
#include <events/application_events.h>
#include <core/service_locator.h>
#include <utilitybox/tools/utility_functions.h>

//#define PARAMETER_PACK_EXPAND(function, args, ...) ((void)function<args>(__VA_ARGS__), ...);
#define PACK_EXPAND(function, args, a...) (int[]) {0, (function<args>(a), 0)...}

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

        template <class ...EventTypes>
        class IEventReceivable : public RequireTypes<EventTypes...> {
            public:
                void OnUpdate();

            protected:
                IEventReceivable(); // Make this class abstract.

            private:
                void ProcessEvents(std::queue<std::shared_ptr<Event*>>& eventData);

                template<class ...AdditionalEventTypes>
                void GetEventOfType(Event* baseEvent);

                template<class CurrentEventType>
                void GetEventOfTypeHelper(Event* baseEvent);

                Events::EventListener<EventTypes...> _eventListener { CallbackFromMemberFn(this, &IEventReceivable::ProcessEvents) };
        };

        template<class ...EventTypes>
        void IEventReceivable<EventTypes...>::ProcessEvents(std::queue<std::shared_ptr<Event *>> &eventData) {
            while (!eventData.empty()) {
                std::shared_ptr<Event*> eventPtr = eventData.front();
                GetEventOfType<EventTypes...>(*eventPtr);
            }
        }

        template<class ...EventTypes>
        template<class... AdditionalEventTypes>
        void IEventReceivable<EventTypes...>::GetEventOfType(Event* baseEvent) {
            PACK_EXPAND(GetEventOfTypeHelper, AdditionalEventTypes, baseEvent);
        }

        template<class... EventTypes>
        template<class CurrentEventType>
        void IEventReceivable<EventTypes...>::GetEventOfTypeHelper(Event *baseEvent) {
            if (auto* derivedEvent = dynamic_cast<CurrentEventType*>(baseEvent)) {
                IEventReceivable::OnEvent(derivedEvent);
            }
        }

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
