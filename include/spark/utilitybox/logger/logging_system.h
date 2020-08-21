
#ifndef SPARK_LOGGING_SYSTEM_H
#define SPARK_LOGGING_SYSTEM_H

#include <spark_pch.h>
#include <utilitybox/logger/logging_interface.h>
#include <utilitybox/logger/log_message_severity.h>
#include <utilitybox/timing/timestamp.h>

namespace Spark {
    namespace UtilityBox {
        namespace Logger {

            struct LogRecord {
                std::string loggingSystemName;
                LogMessageSeverity messageSeverity;
                Timing::TimeStamp timeStamp;
                std::string message;
            };

            class LoggingSystem {
                public:
                    /**
                    * Construct a LoggingSystem instance with a desired name.
                    * @param name - Name for this LoggingSystem.
                    */
                    explicit LoggingSystem(const char* name = "Not provided");

                    /**
                    * Destructor.
                    */
                    ~LoggingSystem() = default;

                    /**
                     * Log a message directly through this LoggingSystem.
                     * @param messageSeverity - Severity of the message.
                     * @param formatString    - Printf-like formatting string for the message and additional parameters
                     * @param ...             - Printf-like list of variadic arguments.
                     */
                    void Log(LogMessageSeverity messageSeverity, const char* formatString, ...) const;

                private:
                    /**
                     * Log a message indirectly through ILoggable interface.
                     * @param messageSeverity - Severity of the message.
                     * @param formatString    - Printf-like formatting string for the message and additional parameters
                     * @param argsList        - Initialized variadic arguments list.
                     */
                    friend class ILoggable;
                    void Log(LogMessageSeverity messageSeverity, const char* formatString, std::va_list argsList) const;

                    class LoggingSystemData;
                    LoggingSystemData* _data;
            };

        } // namespace Logger
    } // namespace UtilityBox
} // namespace Spark


#endif // SPARK_LOGGING_SYSTEM_H
