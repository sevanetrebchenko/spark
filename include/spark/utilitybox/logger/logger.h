
#ifndef SPARK_LOGGER_H
#define SPARK_LOGGER_H

#include <spark/core/rename.h>
#include <spark/utilitybox/logger/adapter/adapter.h>               // IAdapter
#include <spark/utilitybox/logger/logging_hub_interface.h> // ILoggingHub
#include <spark/utilitybox/logger/logging_system.h>              // LogRecord

namespace Spark {
    namespace UtilityBox {
        namespace Logger {

            class LoggingHub : public ILoggingHub {
                public:
                    static LoggingHub* GetInstance();

                    void AttachCustomAdapter(IAdapter *adapter) override;
                    NODISCARD IAdapter* GetCustomAdapter(const char* adapterName) override;
                    void DetachCustomAdapter(IAdapter* adapter) override;

                    NODISCARD const std::chrono::time_point<std::chrono::high_resolution_clock>& GetInitializationTime() override;

                private:
                    LoggingHub();
                    ~LoggingHub();

                    void SendMessage(LogRecord logRecord) override;

                    static LoggingHub* _loggingHub;

                    // Storage for LoggingHub data, back-end functionality, and helper functions.
                    class LoggingHubData;
                    LoggingHubData* _data;
            };

        }
    }
}

#endif // SPARK_LOGGER_H
