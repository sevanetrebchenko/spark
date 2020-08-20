
#ifndef SPARK_EVENT_INTERACTABLE_INTERFACE_H
#define SPARK_EVENT_INTERACTABLE_INTERFACE_H

namespace Spark {
    namespace Events {

        template <class EventType, class ...AdditionalEventTypes>
        struct RequireOnEventFunctionForTypes : RequireOnEventFunctionForTypes<EventType>, RequireOnEventFunctionForTypes<AdditionalEventTypes...> {
            using RequireOnEventFunctionForTypes<EventType>::OnEvent;
        };

        template <class EventType>
        struct RequireOnEventFunctionForTypes<EventType> {
            /**
             * Required overload generated. Takes in an event pointer of the provided EventType.
             * @param eventPointer - Event pointer of the provided EventType.
             */
            virtual void OnEvent(EventType* eventPointer) = 0;
        };

        template <class ChildClass, class ...EventTypes>
        class IEventReceivable : public RequireOnEventFunctionForTypes<EventTypes...> {
            public:
                /**
                 * If 'friend' behavior for the IEventReceivable interface is not desired, overwritten 'OnEvent' functions
                 * must be public in the client implementation. This helper function exists to ensure only events that have
                 * gone through the attached event listener are processed by the client. This function's intended use is
                 * to validate the event pointers received in the client's override of 'OnEvent'.
                 *
                 * Note: if the client implementation of the IEventReceivable interface uses 'friend', this function is
                 * not necessary and the 'OnEvent' functions should be declared private.
                 *
                 * @tparam EventType   - Event type of the provided event pointer.
                 * @param eventPointer - Provided pointer to the event.
                 * @return True  - Given event matches the one passed by this interface (valid event, should be processed accordingly).
                 *         False - Given event does not match the one passed by this interface (invalid event, should not be processed).
                 */
                template <class EventType>
                bool CheckEventPointer(EventType* eventPointer);

            protected:
                IEventReceivable(); // Make this class abstract.

            private:
                class IEventReceivableData;
                IEventReceivableData* _data;
        };
    }
}

#include "../../../src/events/event_interactable_interface.tpp" // Template function includes.

#endif //SPARK_EVENT_INTERACTABLE_INTERFACE_H
