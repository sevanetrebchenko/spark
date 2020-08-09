
#ifndef SPARK_LOG_MESSAGE_H
#define SPARK_LOG_MESSAGE_H

#include <spark_pch.h>               // strrchr, std::vector
#include "../timing/timestamp.h"     // TimeStamp
#include "../tools/global_defines.h" // _NODISCARD_

namespace Spark {
    namespace UtilityBox {
        namespace Logger {

            enum class LogMessageSeverity {
                    DEBUG,
                    WARNING,
                    SEVERE
            };

            class LogMessage {
                public:
                    // Component of intermittent data storage
                    struct LogRecord {
                        /**
                        * Construct a log record to store information about a logged message.
                        * @param message   - Logged message data.
                        * @param timestamp - Timestamp of the logged message.
                        */
                        LogRecord(std::string&& message, Timing::TimeStamp&& timestamp);

                        std::string _message;         // Logged message data.
                        Timing::TimeStamp _timestamp; // Timestamp of the logged message.
                    };

                    /**
                    * Construct an instance of a message to log.
                    * @param messageSeverity - Severity of the message (defaulted to DEBUG).
                    */
                    explicit LogMessage(LogMessageSeverity messageSeverity = LogMessageSeverity::DEBUG);

                    /**
                    * Log message destructor.
                    */
                    ~LogMessage();

                    /**
                    * Supply an additional message to be appended to this message instance. Allows for multi-line messages
                    * by supplying to the same message more than once.
                    * @param formatString - Printf-like formatting string for the message and additional parameters
                    * @param ...          - Printf-like list of variadic arguments.
                    */
                    void Supply(const char* formatString, ...);

                    /**
                    * Get a reference to the store of log record information this LogMessage instance holds.
                    * @return Reference to LogMessage data.
                    */
                    _NODISCARD_ std::vector<LogRecord>& GetLogRecords();

                    /**
                    * Get the severity of this message.
                    * @return Severity of this message.
                    */
                    _NODISCARD_ LogMessageSeverity& GetMessageSeverity();

                    /**
                    * Update the message severity. Can be used anytime before the message is logged.
                    * @param messageSeverity - New message severity.
                    */
                    void SetMessageSeverity(LogMessageSeverity messageSeverity);

                protected:
                    // Allow LoggingSystem to access Supply() overload.
                    friend class LoggingSystem;

                    /**
                    * Supply an additional message to be appended to this message instance. Allows for multi-line messages
                    * by supplying to the same message more than once.
                    * @param formatString - Printf-like formatting string for the message and additional parameters
                    * @param argList      - Printf-like variadic arguments.
                    */
                    void Supply(const char* formatString, std::va_list argList);

                private:

                    // Storage for LogMessage data, back-end functionality, and helper functions.
                    class LogMessageData;
                    LogMessageData* _data;
            };

        } // namespace Logger
    } // namespace UtilityBox
} // namespace Spark

#endif // SPARK_LOG_MESSAGE_H
