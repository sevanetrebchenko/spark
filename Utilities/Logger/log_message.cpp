#include "log_message.h"
#include "../assert_dev.h"
#include <cstdarg>
#include <utility>

namespace UtilityBox {
    namespace Logger {
        class LogMessage::LogMessageBackEnd {
            private:
                unsigned _processingBufferSize;
                char* _processingBuffer;

                struct IntermediateMessage {
                    explicit IntermediateMessage(const char* formatString, ...);
                    const char* GetMessage();
                    ~IntermediateMessage();

                    private:
                        void ProcessIntermediateMessage(const char* formatString, std::va_list argList);
                        unsigned _processingBufferSize;
                        char* _processingBuffer;
                };

                std::vector<const char*> _intermediateMessages;

            public:
                LogMessageBackEnd();
                ~LogMessageBackEnd();
                const char* ProcessMessage(const char* formatString, std::va_list argList);
                const std::vector<const char*>& GetIntermediateProcessingMessages();
                void ClearIntermediateProcessingMessages();
        };

        LogMessage::LogMessageBackEnd::IntermediateMessage::IntermediateMessage(const char *formatString, ...) : _processingBufferSize(64u) {
            _processingBuffer = new (std::nothrow) char[_processingBufferSize];
            ASSERT(ASSERT_LEVEL_FATAL, _processingBuffer != nullptr, "Operation new failed to allocate log message processing buffer - program is out of memory.");

            // write message to buffer
            std::va_list argList;
            va_start(argList, formatString);
            ProcessIntermediateMessage(formatString, argList);
            va_end(argList);
        }

        void LogMessage::LogMessageBackEnd::IntermediateMessage::ProcessIntermediateMessage(const char* formatString, std::va_list argList) {
            // calculate correct number of bytes to write
            unsigned currentBufferSize = _processingBufferSize;

            // copy args list to not modify passed parameters
            std::va_list argsCopy;
            va_copy(argsCopy, argList);
            // If size of the buffer is zero, nothing is written and buffer may be a null pointer, however the return value (number of bytes that would be written not including the null terminator) is still calculated and returned.
            int writeResult = vsnprintf(nullptr, 0, formatString, argsCopy);

            while (_processingBufferSize <= writeResult) {
                _processingBufferSize *= 2;
            }

            // reallocate buffer
            if (currentBufferSize != _processingBufferSize) {
                delete [] _processingBuffer;
                _processingBuffer = new(std::nothrow) char[_processingBufferSize];
                ASSERT(ASSERT_LEVEL_FATAL, _processingBuffer != nullptr, "Operation new failed to re-allocate log message processing buffer - program is out of memory.");
            }

            // write data to buffer
            vsnprintf(_processingBuffer, _processingBufferSize, formatString, argList);
        }

        LogMessage::LogMessageBackEnd::IntermediateMessage::~IntermediateMessage() {
            delete[] _processingBuffer;
        }

        const char *LogMessage::LogMessageBackEnd::IntermediateMessage::GetMessage() {
            return _processingBuffer;
        }

        LogMessage::LogMessageBackEnd::LogMessageBackEnd() : _processingBufferSize(64u) {
            _processingBuffer = new (std::nothrow) char[_processingBufferSize];
            ASSERT(ASSERT_LEVEL_FATAL, _processingBuffer != nullptr, "Operation new failed to allocate log message processing buffer - program is out of memory.");
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
                IntermediateMessage *reallocMessage = new IntermediateMessage("Current processing buffer size %i too small, reallocating to new size %i.", currentBufferSize, _processingBufferSize);
                _intermediateMessages.emplace_back(reallocMessage->GetMessage());
                delete [] _processingBuffer;
                _processingBuffer = new(std::nothrow) char[_processingBufferSize];
                ASSERT(ASSERT_LEVEL_FATAL, _processingBuffer != nullptr, "Operation new failed to re-allocate log message processing buffer - program is out of memory.");
            }

            // write data to buffer
            vsnprintf(_processingBuffer, _processingBufferSize, formatString, argList);

            return _processingBuffer;
        }

        const std::vector<const char *>& LogMessage::LogMessageBackEnd::GetIntermediateProcessingMessages() {
            return _intermediateMessages;
        }

        void LogMessage::LogMessageBackEnd::ClearIntermediateProcessingMessages() {
            _intermediateMessages.clear();
        }

#ifdef DEBUG_MESSAGES
        DBG_LOG_MESSAGE::LogRecord::LogRecord(std::string &&message, Timing::TimeStamp &&timestamp, DBG_LOG_RECORD&& calleeInformation) : _message(std::move(message)),
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

            _logMessages.emplace_back(processedMessage, Timing::TimeStamp(), DBG_LOG_RECORD(std::move(callingFunction), std::move(fileName), lineNumber));
            for (auto* intermediateMessage : _data->GetIntermediateProcessingMessages()) {
                _logMessages.emplace_back(intermediateMessage, Timing::TimeStamp());
            }
            _data->ClearIntermediateProcessingMessages();
        }
#else
        LogMessage::LogRecord::LogRecord(std::string &&message, Timing::TimeStamp &&timestamp) : _message(std::move(message)),
                                                                                     _timestamp(std::move(timestamp)) {
        }

        void LogMessage::Supply(const char* formatString, ...) {
            std::va_list args;
            va_start(args, formatString);
            const char* processedMessage = _data->ProcessMessage(formatString, args);
            va_end(args);

            _logMessages.emplace_back(processedMessage, Timing::TimeStamp());
            for (auto* intermediateMessage : _data->GetIntermediateProcessingMessages()) {
                _logMessages.emplace_back(intermediateMessage, Timing::TimeStamp());
            }
            _data->ClearIntermediateProcessingMessages();
        }
#endif
    }
}
