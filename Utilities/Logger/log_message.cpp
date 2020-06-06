
#include "log_message.h"   // LogMessage functions
#include "../assert_dev.h" // Asserts
#include <cstdarg>         // std::va_list, va_copy, va_start, va_end

namespace UtilityBox {
    namespace Logger {
        //--------------------------------------------------------------------------------------------------------------
        // LOG MESSAGE DATA
        //--------------------------------------------------------------------------------------------------------------
        class LogMessage::LogMessageData {
            public:
                /**
                 * Allocates the necessary buffers to be able to process log messages.
                 */
                LogMessageData();

                /**
                 * Cleans up information related to message processing.
                 */
                ~LogMessageData();

                /**
                 * Helper function to process printf-like message with variadic arguments.
                 * @param formatString - Printf-like format string for log messages.
                 * @param argList      - Printf-like variadic arguments.
                 * @return Pointer to the buffer which holds the processed message.
                 */
                const char* ProcessMessage(const char* formatString, std::va_list argList);

            private:
                unsigned _processingBufferSize; // Size of the processing buffer.
                char* _processingBuffer;        // Pointer to the processing buffer.
        };

        // Allocates the necessary buffers to be able to process log messages.
        LogMessage::LogMessageData::LogMessageData() : _processingBufferSize(64u) {
            // Start buffer with (baseline) 64 byte size.
            _processingBuffer = new (std::nothrow) char[_processingBufferSize];
            ASSERT(_processingBuffer != nullptr, "Operation new failed to allocate log message processing buffer - program is out of memory.");
        }

        // Cleans up information related to message processing.
        LogMessage::LogMessageData::~LogMessageData() {
            delete [] _processingBuffer;
        }

        // Helper function to process printf-like message with variadic arguments.
        const char* LogMessage::LogMessageData::ProcessMessage(const char* formatString, std::va_list argList) {
            unsigned currentBufferSize = _processingBufferSize;

            // Copy args list to not modify passed parameters (yet).
            std::va_list argsCopy;
            va_copy(argsCopy, argList);

            // If size of the buffer is zero, nothing is written and buffer may be a null pointer, however the return
            // value (number of bytes that would be written not including the null terminator) is still calculated and returned.
            int writeResult = vsnprintf(nullptr, 0, formatString, argsCopy);

            // If buffer size is equal to write result, there will not be space for the null terminator for the string.
            // Multiple buffer size by two to adequately house string in a new buffer.
            while (_processingBufferSize <= writeResult) {
                _processingBufferSize *= 2;
            }

            // Reallocate buffer.
            if (currentBufferSize != _processingBufferSize) {
                delete [] _processingBuffer;
                _processingBuffer = new(std::nothrow) char[_processingBufferSize];
                ASSERT(_processingBuffer != nullptr, "Operation new failed to re-allocate log message processing buffer - program is out of memory.");
            }

            vsnprintf(_processingBuffer, _processingBufferSize, formatString, argList);
            return _processingBuffer;
        }


        //--------------------------------------------------------------------------------------------------------------
        // LOG MESSAGE
        //--------------------------------------------------------------------------------------------------------------
        // Construct an instance of a message to log.
        LogMessage::LogMessage(LogMessageSeverity messageSeverity) : _messageSeverity(messageSeverity), _data(std::make_unique<LogMessageData>()) {
            // Nothing to do here.
        }

        // Log message destructor.
        LogMessage::~LogMessage() {
            _data.reset();
        }

        // Get a reference to the store of log record information this LogMessage instance holds.
        std::vector<LogMessage::LogRecord>& LogMessage::GetLogRecords() {
            return _logMessages;
        }

        // Get the severity of this message.
        LogMessageSeverity& LogMessage::GetMessageSeverity() {
            return _messageSeverity;
        }

    #ifdef DEBUG_MESSAGES
        // Construct a debug log record to store callee information about a logged message.
        DBG_LOG_MESSAGE::DBG_LOG_RECORD::DBG_LOG_RECORD(std::string &&filename, std::string &&functionName, int lineNumber) : _fileName(std::move(filename)),
                                                                                                                              _functionName(std::move(functionName)),
                                                                                                                              _lineNumber(lineNumber) {
            // Nothing to do here.
        }

        // Construct a log record to store information about a logged message.
        DBG_LOG_MESSAGE::LogRecord::LogRecord(std::string &&message, Timing::TimeStamp &&timestamp, DBG_LOG_RECORD&& calleeInformation) : _message(std::move(message)),
                                                                                                                                          _timestamp(std::move(timestamp)),
                                                                                                                                          _calleeInformation(std::move(calleeInformation)) {
            // Nothing to do here.
        }

        // Supply an additional message to be appended to this message instance. Allows for multi-line messages by
        // supplying to the same message more than once.
        void DBG_LOG_MESSAGE::SUPPLY_DBG(std::string&& callingFunction, std::string&& fileName, int lineNumber, const char* formatString, ...) {
            std::va_list args;
            va_start(args, formatString);
            const char* processedMessage = _data->ProcessMessage(formatString, args);
            va_end(args);

            _logMessages.emplace_back(processedMessage, Timing::TimeStamp(), DBG_LOG_RECORD(std::move(callingFunction), std::move(fileName), lineNumber));
        }
    #else
        // Construct an instance of a message to log.
        LogMessage::LogRecord::LogRecord(std::string&& message, Timing::TimeStamp&& timestamp) : _message(std::move(message)),
                                                                                                 _timestamp(std::move(timestamp)) {
            // Nothing to do here.
        }

        // Supply an additional message to be appended to this message instance. Allows for multi-line messages by
        // supplying to the same message more than once.
        void LogMessage::Supply(const char* formatString, ...) {
            std::va_list args;
            va_start(args, formatString);
            const char* processedMessage = _data->ProcessMessage(formatString, args);
            va_end(args);

            _logMessages.emplace_back(processedMessage, Timing::TimeStamp());
        }
    #endif
    }
}
