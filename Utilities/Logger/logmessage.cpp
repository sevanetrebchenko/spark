#include "logmessage.h"
#include <vector>
#include "timestamp.h"
#include <cstdarg>

namespace UtilityBox::Logger {
    class LogMessage::LogMessageData {
        private:
            unsigned _processingBufferSize;
            char* _processingBuffer;
#ifdef DEBUG_MESSAGES
        struct LogDebugInformation {
            LogDebugInformation(std::string&& file, std::string&& function, unsigned line);
            std::string _file;
            std::string _function;
            unsigned _line;
        };
#endif
        public:
            explicit LogMessageData(LogMessageSeverity messageSeverity);
            ~LogMessageData();
#ifdef DEBUG_MESSAGES
            void ProcessMessage(std::string&& file, std::string&& function, unsigned line, const char* formatString, std::va_list argList);
#else
            void ProcessMessage(const char* formatString, std::va_list argList);
#endif

            std::vector<std::string> _messages;
            std::vector<std::string> _loggingSystemNames;
            std::vector<Timing::TimeStamp> _timestampData;
            LogMessageSeverity _messageSeverity;
#ifdef DEBUG_MESSAGES
            std::vector<LogDebugInformation> _calleeInformation;
#endif
    };

    LogMessage::LogMessageData::LogMessageData(LogMessageSeverity messageSeverity) : _processingBufferSize(64u), _messageSeverity(messageSeverity) {
        _processingBuffer = new (std::nothrow) char[_processingBufferSize];
        // TODO: assert
    }

    LogMessage::LogMessageData::~LogMessageData() {
        delete [] _processingBuffer;
        // TODO: _formattedMessages.clear();
    }

    // LogMessage functions begin
    LogMessage::LogMessage(LogMessageSeverity messageSeverity) : _data(std::make_unique<LogMessageData>(messageSeverity)) {
    }

    // Functions that are different (overloaded by macro) between the two classes
#ifdef DEBUG_MESSAGES
    DBG_LOG_MESSAGE::LogMessageData::LogDebugInformation::LogDebugInformation(std::string &&file, std::string &&function, unsigned int line) : _file(std::move(file)), _function(std::move(function)), _line(line) {
    }

    void DBG_LOG_MESSAGE::SUPPLY_DBG(std::string&& callingFunction, std::string&& fileName, int lineNumber, const char* formatString, ...) {
        std::va_list args;
        va_start(args, formatString);
        _data->ProcessMessage(std::move(callingFunction), std::move(fileName), lineNumber, formatString, args);
        va_end(args);
    }

    void LogMessage::LogMessageData::ProcessMessage(std::string&& file, std::string&& function, unsigned line, const char* formatString, std::va_list argList) {
        // record callee information
        _calleeInformation.emplace_back(std::move(file), std::move(function), line);

        // calculate correct number of bytes to write
        unsigned currentBufferSize = _processingBufferSize;

        // copy args list to not modify passed parameters
        std::va_list argsCopy;
        va_copy(argsCopy, argList);
        // If size of the buffer is zero, nothing is written and buffer may be a null pointer, however the return value (number of bytes that would be written not including the null terminator) is still calculated and returned.
        int writeResult = vsnprintf(nullptr, 0, formatString, argsCopy);

        // get buffer size of powers of 2 to match the size correctly
        // if buffer size is equal to write result, there will not be space for the null terminator for the string
        while (_processingBufferSize <= writeResult) {
            _processingBufferSize *= 2;
        }

        // reallocate buffer
        if (currentBufferSize != _processingBufferSize) {
            delete [] _processingBuffer;
            _processingBuffer = new char[_processingBufferSize];
        }

        // write data to buffer
        vsnprintf(_processingBuffer, _processingBufferSize, formatString, argList);

        _messages.emplace_back(_processingBuffer);
    }
#else
    void LogMessage::Supply(const char* formatString, ...) {
        std::va_list args;
        va_start(args, formatString);
        _data->ProcessMessage(formatString, args);
        va_end(args);
    }

    void LogMessage::LogMessageData::ProcessMessage(const char* formatString, std::va_list argList) {
        // record timestamp
        _timestampData.emplace_back();

        // calculate correct number of bytes to write
        unsigned currentBufferSize = _processingBufferSize;

        // copy args list to not modify passed parameters
        std::va_list argsCopy;
        va_copy(argsCopy, argList);
        // If size of the buffer is zero, nothing is written and buffer may be a null pointer, however the return value (number of bytes that would be written not including the null terminator) is still calculated and returned.
        int writeResult = vsnprintf(nullptr, 0, formatString, argsCopy);

        // get buffer size of powers of 2 to match the size correctly
        // if buffer size is equal to write result, there will not be space for the null terminator for the string
        while (_processingBufferSize <= writeResult) {
            _processingBufferSize *= 2;
        }

        // reallocate buffer
        if (currentBufferSize != _processingBufferSize) {
            delete [] _processingBuffer;
            _processingBuffer = new char[_processingBufferSize];
        }

        // write data to buffer
        vsnprintf(_processingBuffer, _processingBufferSize, formatString, argList);

        _messages.emplace_back(_processingBuffer);

        // place empty string
        _loggingSystemNames.emplace_back("\0");
    }
#endif
}
