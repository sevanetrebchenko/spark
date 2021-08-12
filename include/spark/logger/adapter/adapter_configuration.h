
#ifndef SPARK_ADAPTER_CONFIGURATION_H
#define SPARK_ADAPTER_CONFIGURATION_H

#include "spark/logger/log_record.h"

namespace Spark {
    namespace Logger {

        struct AdapterConfiguration {
            explicit AdapterConfiguration(std::string  name);

            // Format string for the entire log message.
            // Format specifiers:
            //      %t - timestamp
            //      %c - log count
            //      %d - calendar date
            //      %s - message severity
            //      %f - file, function, and line number
            //      %x - separator
            //      %m - message
            std::string globalFormattingString;

            // Format string for log message timestamps.
            // Default format: 000m 00s 0000ms
            // Format specifiers:
            //      %m - minutes
            //      %s - seconds
            //      %l - milliseconds
            std::string timestampFormattingString;

            // Format string for calendar date of log message.
            // Default: DAY 01, MONTH 0000 - 00:00:00
            // Format specifiers: strftime
            std::string calendarFormatString;

            // Format string for log message location (file, function, line number)
            // Default: file, function:line number
            // Format specifiers:
            //      %f - file
            //      %u - function
            //      %l - line number
            std::string locationFormatString;

            std::string name;
            LogSeverity severityCutoff;
            bool wrapMessage;
            std::size_t wrapLimit;
            std::size_t separatorLength;
        };

    }
}

#endif //SPARK_ADAPTER_CONFIGURATION_H
