
#ifndef DATASTRUCTURES_LOGGER_H
#define DATASTRUCTURES_LOGGER_H

#include "assert_dev.h"
#include "log_message.h"
#include <fstream>
#include <cstring>


namespace UtilityBox {
    namespace Logger {
        void Log(LogMessage *msg);
    }
}

//        enum LoggerLevel {
//            ALL = 0, // everything
//            VERBOSE, // warnings and errors
//            MINIMAL  // only errors
//        }

//
//        void SetLogFile(const std::string &newFilename);
//        void SetLoggingLevel(LoggerLevel level);
//        void PrintSampleLogMessage();
//        void UpdateBufferSize();

#endif //DATASTRUCTURES_LOGGER_H
