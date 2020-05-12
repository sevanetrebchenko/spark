#ifndef DATASTRUCTURES_LOG_MESSAGE_H
#define DATASTRUCTURES_LOG_MESSAGE_H

#include <ostream>
#include <vector>
#include <string>
#include <cstdarg>

enum LogMessageSeverity {
    DEBUG,
    WARNING,
    ERROR
};

class LogMessage {
    public:
        LogMessage();
        ~LogMessage();

        void Supply(LogMessageSeverity, const char* formatString, ...);
        friend std::ostream &operator<<(std::ostream &, LogMessage &);

    protected:
        void ProcessMessage(const char* formatString, std::va_list args);

        char* _logBuffer;
        unsigned _logBufferSize;
        std::vector<std::string> _messages;
};

#endif //DATASTRUCTURES_LOG_MESSAGE_H
