
#ifndef SPARK_EVENT_DEFINITIONS_H
#define SPARK_EVENT_DEFINITIONS_H

#include "spark/utility.h"

namespace Spark {
    namespace Events {

        class IEvent; // Forward declaration.

        typedef std::uint32_t EventTypeID;
        typedef std::shared_ptr<const IEvent> EventHandle;

        #define REGISTER_EVENT(ClassName)                                                \
        public:                                                                          \
            static constexpr ::Spark::Events::EventTypeID ID = STRINGHASH(#ClassName);   \
            static constexpr const char* Name = #ClassName;                              \
            NODISCARD ::Spark::Events::EventTypeID GetID() const override { return ID; }

    }
}

#endif //SPARK_EVENT_DEFINITIONS_H
