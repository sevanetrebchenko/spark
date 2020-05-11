
#ifndef DATASTRUCTURES_LOGGER_H
#define DATASTRUCTURES_LOGGER_H

#include <string>

namespace UtilityBox {
    namespace Logger {
        enum LoggerLevel {
            ALL = 0, // everything
            VERBOSE, // warnings and errors
            MINIMAL  // only errors
        };

        enum MessageSeverity {
            DEBUG,
            WARNING,
            ERROR
        };

        void LogMessage(MessageSeverity, const char* formatString, ...);

        // support for block messages
        unsigned GetOpenBlockID();
        void StartMessageBlock(unsigned blockID);
        void LogBlockMessage(unsigned blockID, MessageSeverity severity, const char* formatString, ...);
        void EndMessageBlock(unsigned blockID);


        void SetLogFile(const std::string &newFilename);

        void SetLoggingLevel(LoggerLevel level);
        void PrintSampleLogMessage();
        void UpdateBufferSize();
    }
}


#endif //DATASTRUCTURES_LOGGER_H
