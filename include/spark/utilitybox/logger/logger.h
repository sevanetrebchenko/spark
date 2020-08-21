
#ifndef SPARK_LOGGER_H
#define SPARK_LOGGER_H

#include <spark_pch.h>                        // std::chrono
#include <utilitybox/logger/logging_system.h> // LoggingSystem
#include <utilitybox/logger/adapter.h>        // Adapter
#include <utilitybox/tools/global_defines.h>  // _NODISCARD_
#include <utilitybox/logger/logging_hub_interface.h>

namespace Spark {
    namespace UtilityBox {
        namespace Logger {

            class LoggingHub : public ILoggingHub {
                public:
                    static LoggingHub* GetInstance();

                    void AttachCustomAdapter(Adapter *adapter) override;
                    _NODISCARD_ Adapter* GetCustomAdapter(const char* adapterName) override;
                    void DetachCustomAdapter(Adapter* adapter) override;

                    _NODISCARD_ const std::chrono::time_point<std::chrono::high_resolution_clock>& GetInitializationTime() override;

                private:
                    LoggingHub();
                    ~LoggingHub();

                    void SendMessage(LogRecord logRecord) override;

                    static LoggingHub *_loggingHub; // Singleton.

                    // Storage for LoggingHub data, back-end functionality, and helper functions.
                    class LoggingHubData;
                    LoggingHubData *_data;
            };

        } // namespace Logger
    } // namespace UtilityBox
} // namespace Spark

#endif // SPARK_LOGGER_H
