
#ifndef SPARK_LOGGING_HUB_INTERFACE_H
#define SPARK_LOGGING_HUB_INTERFACE_H

#include <spark/core/core.h>
#include <spark/utilitybox/logger/adapter/adapter.h>       // IAdapter
#include <spark/utilitybox/logger/logging_system.h>      // LoggingSystem

namespace Spark {
    namespace UtilityBox {
        namespace Logger {

            class ILoggingHub {
                public:
                    virtual void AttachCustomAdapter(IAdapter* adapter) = 0;
                    _NODISCARD_ virtual IAdapter* GetCustomAdapter(const char* name) = 0;
                    virtual void DetachCustomAdapter(IAdapter* adapter) = 0;

                    _NODISCARD_ virtual const std::chrono::time_point<std::chrono::high_resolution_clock>& GetInitializationTime() = 0;

                private:
                    friend class LoggingSystem;
                    virtual void SendMessage(LogRecord logRecord) = 0;
            };

        }
    }
}

#endif // SPARK_LOGGING_HUB_INTERFACE_H
