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

        struct LoggerConfiguration {
            enum BlockMessageIndent {
                NONE,
                HALF,
                FULL
            };

            enum LoggerLevel {
                ALL = 0, // everything
                VERBOSE, // warnings and errors
                MINIMAL  // only errors
            };

            // HEADER
            bool includeMessageNum = true;
            bool includeDateTime = true;

            // LOG MESSAGE
            bool separateMessages = false;
            bool includeMessageSeverity = true;
            bool includeMessageTimestamp = true;
            bool printMessagesOnDestroy = false;

            std::string calendarHeaderFormat = "[%A %d, %B %Y - %H:%M:%S]";
            LoggerLevel level;
            BlockMessageIndent indent;
        };

        enum LogMessageSeverity {
            DEBUG,
            WARNING,
            ERROR
        };

        /**
         * A message that spans a single line in any stream. Automatically deleted once printed.
         */
        class SingleLineMessage {
            public:
                /**
                 * Construct a message that takes up one line.
                 * @param formatString Custom header format string (printf-style)
                 * @param ... Supplied data for format string (optional, printf-style)
                 */
                SingleLineMessage(LogMessageSeverity, const char* formatString, ...);

                /**
                 * Destroys
                 */
                ~SingleLineMessage();
                unsigned GetDataIndex();

                // log message to a different stream
                friend std::ostream& operator<<(std::ostream&, const SingleLineMessage&);
            private:
                unsigned _dataIndex;
        };

        /**
         * A message that spans multiple lines in any stream. Automatically deleted once printed.
         */
        class MultiLineMessage {
            public:
                /**
                 * Construct an instance of a message that spans multiple lines with a default header.
                 */
                MultiLineMessage();

                /**
                 * Construct an instance of a message that spans multiple lines with a custom message header.
                 * @param formatString Custom header format string (printf-style)
                 * @param ... Supplied data for format string (optional, printf-style)
                 */
                MultiLineMessage(LogMessageSeverity, const char* formatString, ...);
                void Supply(LogMessageSeverity, const char* formatString, ...) const;
                ~MultiLineMessage();
                unsigned GetDataIndex();

                // log message to a different stream
                friend std::ostream& operator<<(std::ostream&, const MultiLineMessage&);
            private:
                unsigned _dataIndex;
        };

        _NODISCARD_ LoggerConfiguration& GetLoggerConfig();
        void Log(SingleLineMessage *msg);
        void Log(LogMessageSeverity, const char* formatString, ...);
        void Log(MultiLineMessage *msg);

    }
}

//
//        void SetLogFile(const std::string &newFilename);
//        void SetLoggingLevel(LoggerLevel level);
//        void PrintSampleLogMessage();
//        void UpdateBufferSize();

#endif //DATASTRUCTURES_LOGGER_H
