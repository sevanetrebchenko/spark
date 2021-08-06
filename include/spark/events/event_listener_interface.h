
#ifndef SPARK_EVENT_LISTENER_INTERFACE_H
#define SPARK_EVENT_LISTENER_INTERFACE_H

#include <spark/core/rename.h>
#include <spark/events/types/event.h>

namespace Spark {
    namespace Events {

        class IEventListener {
            public:
                NODISCARD virtual bool OnEventReceived(std::shared_ptr<Events::IEvent*> eventPointer) = 0;
                NODISCARD virtual const char* GetName() = 0;
                NODISCARD virtual const std::vector<std::string>& GetEventTypesAsStrings() = 0;

                virtual void OnUpdate() = 0;
        };

    }
}

#endif //SPARK_EVENT_LISTENER_INTERFACE_H
