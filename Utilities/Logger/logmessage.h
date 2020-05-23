#ifndef DATASTRUCTURES_LOGMESSAGE_H
#define DATASTRUCTURES_LOGMESSAGE_H
#include <ostream>
#include <memory>

#define DEBUG_MESSAGES

namespace UtilityBox::Logger {
    enum LogMessageSeverity {
        DEBUG,
        WARNING,
        SEVERE
    };

    class LogMessage {
        public:
            explicit LogMessage(LogMessageSeverity messageSeverity = DEBUG);
            ~LogMessage();
            void Supply(const char* formatString, ...);

        private:
            // Opaque pointer to abstract data - data should not be available
            class LogMessageData;
            std::unique_ptr<LogMessageData> _data;

            // Allow the logging system processing these messages to access the data
            friend class LoggingSystem;
    };

#ifdef DEBUG_MESSAGES
    // Class captures caller and records debug information
    class DBG_LOG_MESSAGE {
        public:
            explicit DBG_LOG_MESSAGE(LogMessageSeverity messageSeverity = DEBUG);
            ~DBG_LOG_MESSAGE();
            void SUPPLY_DBG(std::string&& callingFunction, std::string&& fileName, int lineNumber, const char* formatString, ...);

        private:
            // Opaque pointer to abstract data
            class LogMessageData;
            std::unique_ptr<LogMessageData> _data;

            // Allow the logging system processing these messages to access the data
            friend class LoggingSystem;
    };

// shorten file name - get the last occurence of the '/' character to only get the file name
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#undef LogMessage
#define LogMessage DBG_LOG_MESSAGE
#undef Supply
#define Supply(formatString, ...) SUPPLY_DBG(__PRETTY_FUNCTION__, __FILENAME__, __LINE__, formatString, ##__VA_ARGS__)
#endif
}

#endif //DATASTRUCTURES_LOGMESSAGE_H
