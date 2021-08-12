
#ifndef SPARK_LOG_RECORD_H
#define SPARK_LOG_RECORD_H

#include "spark/logger/timestamp.h"

namespace Spark {
    namespace Logger {

        enum class LogSeverity {
            DEBUG,
            WARNING,
            ERROR
        };

        struct LogRecord {
            // File information.
            std::string filename;
            std::string function;
            int lineNumber;

            // Message information.
            LogSeverity messageSeverity;
            TimeStamp timeStamp;
            std::string message;
        };

    }
}

#endif //SPARK_LOG_RECORD_H
