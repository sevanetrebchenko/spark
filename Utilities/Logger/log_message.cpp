#include "log_message.h"
#include <cstdarg>
#include <utility>

namespace UtilityBox {
    namespace Logger {
        class LogMessage::LogMessageBackEnd {
            private:
                unsigned _processingBufferSize;
                char* _processingBuffer;

            public:
                LogMessageBackEnd();
                ~LogMessageBackEnd();
                const char* ProcessMessage(const char* formatString, std::va_list argList);
        };

        LogMessage::LogMessageBackEnd::LogMessageBackEnd() : _processingBufferSize(64u) {
            _processingBuffer = new (std::nothrow) char[_processingBufferSize];
            // TODO: assert
        }

        LogMessage::LogMessageBackEnd::~LogMessageBackEnd() {
            delete [] _processingBuffer;
        }

        // LogMessage functions begin
        LogMessage::LogMessage(LogMessageSeverity messageSeverity) : _messageSeverity(messageSeverity), _data(std::make_unique<LogMessageBackEnd>()) {
        }

        LogMessage::~LogMessage() {
            _data.reset();
        }

        std::vector<LogMessage::LogRecord> *LogMessage::GetLogMessages() {
            return &_logMessages;
        }

        LogMessageSeverity LogMessage::GetMessageSeverity() {
            return _messageSeverity;
        }

        const char* LogMessage::LogMessageBackEnd::ProcessMessage(const char* formatString, std::va_list argList) {
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

            return _processingBuffer;
        }

#ifdef DEBUG_MESSAGES
        DBG_LOG_MESSAGE::LogRecord::LogRecord(std::string &&message, std::string&& loggingSystemName, Timing::TimeStamp &&timestamp, DBG_LOG_RECORD&& calleeInformation) : _message(std::move(message)),
                                                                                                                                                                           _loggingSystemName(std::move(loggingSystemName)),
                                                                                                                                                                           _timestamp(std::move(timestamp)),
                                                                                                                                                                           _calleeInformation(std::move(calleeInformation)) {
        }

        DBG_LOG_MESSAGE::DBG_LOG_RECORD::DBG_LOG_RECORD(std::string &&filename, std::string &&functionName, int lineNumber) : _fileName(std::move(filename)),
                                                                                                                              _functionName(std::move(functionName)),
                                                                                                                              _lineNumber(lineNumber) {
        }

        void DBG_LOG_MESSAGE::SUPPLY_DBG(std::string&& callingFunction, std::string&& fileName, int lineNumber, const char* formatString, ...) {
            std::va_list args;
            va_start(args, formatString);
            const char* processedMessage = _data->ProcessMessage(formatString, args);
            va_end(args);

            _logMessages.emplace_back(processedMessage, "", Timing::TimeStamp(), DBG_LOG_RECORD(std::move(callingFunction), std::move(fileName), lineNumber));
        }
#else
        LogMessage::LogRecord::LogRecord(std::string &&message, std::string &&loggingSystemName, Timing::TimeStamp &&timestamp) : _message(std::move(message)),
                                                                                                                                  _loggingSystemName(std::move(loggingSystemName)),
                                                                                                                                  _timestamp(std::move(timestamp)) {
        }

        void LogMessage::Supply(const char* formatString, ...) {
            std::va_list args;
            va_start(args, formatString);
            const char* processedMessage = _data->ProcessMessage(formatString, args);
            va_end(args);

            _logMessages.emplace_back(processedMessage, "", Timing::TimeStamp());
        }
#endif
    }
}
