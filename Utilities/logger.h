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

#define __NO_DISCARD__ [[nodiscard]]

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

            std::string calendarHeaderFormat = "[%A %d, %B %Y - %H:%M:%S]";
        };

        enum LogMessageSeverity {
            DEBUG,
            WARNING,
            ERROR
        };

        class SingleLineMessage {
            public:
                SingleLineMessage(LogMessageSeverity, const char* formatString, ...);
                ~SingleLineMessage();

                // log message to a different stream
                friend std::ostream& operator<<(std::ostream&, const SingleLineMessage&);
            private:
                unsigned _dataIndex;
        };

        class MultiLineMessage {
            public:
                MultiLineMessage();
                MultiLineMessage(LogMessageSeverity, const char* formatString, ...);
                void Supply(LogMessageSeverity, const char* formatString, ...) const;
                ~MultiLineMessage();

                // log message to a different stream
                friend std::ostream& operator<<(std::ostream&, const MultiLineMessage&);
            private:
                unsigned _dataIndex;
        };

        __NO_DISCARD__ LoggerConfiguration& GetLoggerConfig();
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
