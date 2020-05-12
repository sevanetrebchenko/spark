
#ifndef DATASTRUCTURES_LOG_MESSAGE_CPP
#define DATASTRUCTURES_LOG_MESSAGE_CPP

#include "log_message.h"

// LogMessage class
LogMessage::LogMessage() {
    _logBufferSize = 256;
    _logBuffer = new(std::nothrow) char[_logBufferSize];
    // TODO: Assert
}

void LogMessage::ProcessMessage(const char *formatString, std::va_list args) {
    // calculate correct number of bytes to write
    unsigned currentBufferSize = _logBufferSize;

    // copy args list to not modify passed parameters
    std::va_list argsCopy;
    va_copy(argsCopy, args);
    // If size of the buffer is zero, nothing is written and buffer may be a null pointer, however the return value (number of bytes that would be written not including the null terminator) is still calculated and returned.
    int writeResult = vsnprintf(nullptr, 0, formatString, argsCopy);

    // get buffer size of powers of 2 to match the size correctly
    // if buffer size is equal to write result, there will not be space for the null terminator for the string
    while (_logBufferSize <= writeResult) {
        _logBufferSize *= 2;
    }

    // reallocate buffer
    if (currentBufferSize != _logBufferSize) {
        delete [] _logBuffer;
        _logBuffer = new char[_logBufferSize];
    }

    // write data to buffer
    vsnprintf(_logBuffer, _logBufferSize, formatString, args);
}

std::ostream &operator<<(std::ostream &stream, LogMessage &toLog) {
    if (!toLog._messages.empty()) {
        for (auto& message : toLog._messages) {
            stream << message << std::endl;
        }
    }

    delete &toLog;
    return stream;
}

LogMessage::~LogMessage() {
    delete [] _logBuffer;
    _messages.clear();
}

void LogMessage::Supply(LogMessageSeverity, const char *formatString, ...) {
    std::va_list args;
    va_start(args, formatString);
    ProcessMessage(formatString, args);
    va_end(args);

    // record message
    _messages.emplace_back(_logBuffer);
}

#endif
