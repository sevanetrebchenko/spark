
#ifndef SPARK_LOGGING_HUB_INTERFACE_H
#define SPARK_LOGGING_HUB_INTERFACE_H

#include <utilitybox/logger/logging_system.h>
#include <utilitybox/logger/adapter.h>
#include <utilitybox/tools/global_defines.h>

namespace Spark {
    namespace UtilityBox {
        namespace Logger {

            class ILoggingHub {
                public:
                    virtual void AttachCustomAdapter(Adapter* adapter) = 0;
                    _NODISCARD_ virtual Adapter* GetCustomAdapter(const char* name) = 0;
                    virtual void DetachCustomAdapter(Adapter* adapter) = 0;

                    _NODISCARD_ virtual const std::chrono::time_point<std::chrono::high_resolution_clock>& GetInitializationTime() = 0;

                private:
                    friend class LoggingSystem;
                    virtual void SendMessage(LogRecord logRecord) = 0;
            };

        }
    }
}

#endif //SPARK_LOGGING_HUB_INTERFACE_H
