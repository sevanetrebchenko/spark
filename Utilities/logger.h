#pragma once
#ifndef DATASTRUCTURES_LOGGER_H
#define DATASTRUCTURES_LOGGER_H

#include "assert_dev.h"
#include <fstream>
#include <cstring>
#include <string>
#include <chrono>
#include <vector>
#include <iomanip>

#define _NODISCARD_ [[nodiscard]]

namespace UtilityBox {
    namespace Logger {

        enum LogMessageSeverity {
            DEBUG,
            WARNING,
            ERROR
        };

        class LogMessage {
            public:
                /**
                 * Constructor.
                 * @param messageSeverity - Severity of message.
                 */
                explicit LogMessage(LogMessageSeverity messageSeverity);

                /**
                 * Constructor.
                 * @param messageSeverity   - Severity of message.
                 * @param formatString      - Format string (printf-style).
                 * @param ...               - Additional parameters (printf-style).
                 */
                LogMessage(LogMessageSeverity messageSeverity, const char* formatString, ...);

                /**
                 * Destructor.
                 */
                ~LogMessage();

                /**
                 * Provide an additional message. Can be done multiple times for multi-line messages.
                 * @param formatString  - Format string (printf-style)
                 * @param ...           - Additional parameters (printf-style)
                 */
                void Supply(const char* formatString, ...);

            private:
                // Opaque pointer to abstract data - data should not be available
                class LogMessageData;
                std::unique_ptr<LogMessageData> _data;

                // Allow the logging system processing these messages to access the data
                friend class LoggingSystem;
        };

        class LoggingSystem {
            public:
                /**
                 * Constructor.
                 * @param name - Name for the new logging system. System name will be attached to log messages that are
                 *               sent through this instance.
                 */
                explicit LoggingSystem(std::string&& name = "");

                /**
                 * Destructor.
                 */
                ~LoggingSystem();

                /**
                 * Log an instance of the message class.
                 * @param message
                 */
                void Log(LogMessage* message);

                /**
                 * Log a message directly.
                 * @param messageSeverity
                 * @param formatString
                 * @param ...
                 */
                void Log(LogMessageSeverity messageSeverity, const char* formatString, ...);
            private:
                // Opaque pointer to abstract data
                struct LoggingSystemData;
                std::unique_ptr<LoggingSystemData> _data;

//                friend class LoggingHub;
        };

//        struct LoggerConfiguration {
//            enum BlockMessageIndent {
//                NONE,
//                HALF,
//                FULL
//            };
//
//            enum LoggerLevel {
//                ALL = 0, // everything
//                VERBOSE, // warnings and errors
//                MINIMAL  // only errors
//            };
//
//            // HEADER
//            bool includeMessageNum = true;
//            bool includeDateTime = true;
//
//            // LOG MESSAGE
//            bool separateMessages = false;
//            bool includeMessageSeverity = true;
//            bool includeMessageTimestamp = true;
//            bool printMessagesOnDestroy = false;
//
//            std::string calendarHeaderFormat = "[%A %d, %B %Y - %H:%M:%S]";
//            LoggerLevel level;
//            BlockMessageIndent indent;
//        };
//
//        enum LogMessageSeverity {
//            DEBUG,
//            WARNING,
//            ERROR
//        };
//
//        /**
//         * A message that spans a single line in any stream. Automatically deleted once printed.
//         */
//        class SingleLineMessage {
//            public:
//                /**
//                 * Construct a message that takes up one line.
//                 * @param formatString Custom header format string (printf-style)
//                 * @param ... Supplied data for format string (optional, printf-style)
//                 */
//                SingleLineMessage(LogMessageSeverity, const char* formatString, ...);
//
//                /**
//                 * Destroys
//                 */
//                ~SingleLineMessage();
//                unsigned GetDataIndex();
//
//                // log message to a different stream
//                friend std::ostream& operator<<(std::ostream&, const SingleLineMessage&);
//            private:
//                unsigned _dataIndex;
//        };
//
//        /**
//         * A message that spans multiple lines in any stream. Automatically deleted once printed.
//         */
//        class MultiLineMessage {
//            public:
//                /**
//                 * Construct an instance of a message that spans multiple lines with a default header.
//                 */
//                MultiLineMessage();
//
//                /**
//                 * Construct an instance of a message that spans multiple lines with a custom message header.
//                 * @param formatString Custom header format string (printf-style)
//                 * @param ... Supplied data for format string (optional, printf-style)
//                 */
//                MultiLineMessage(LogMessageSeverity, const char* formatString, ...);
//                void Supply(LogMessageSeverity, const char* formatString, ...) const;
//                ~MultiLineMessage();
//                unsigned GetDataIndex();
//
//                // log message to a different stream
//                friend std::ostream& operator<<(std::ostream&, const MultiLineMessage&);
//            private:
//                unsigned _dataIndex;
//        };
//
//        _NODISCARD_ LoggerConfiguration& GetLoggerConfig();
//        void Log(SingleLineMessage *msg);
//        void Log(LogMessageSeverity, const char* formatString, ...);
//        void Log(MultiLineMessage *msg);

    }
}

//
//        void SetLogFile(const std::string &newFilename);
//        void SetLoggingLevel(LoggerLevel level);
//        void PrintSampleLogMessage();
//        void UpdateBufferSize();

#endif //DATASTRUCTURES_LOGGER_H
