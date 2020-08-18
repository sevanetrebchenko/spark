
#include <spark_pch.h>                       // std::va_list, va_copy, va_start, va_end
#include <utilitybox/logger/log_message.h>   // LogMessage functions
#include <utilitybox/tools/assert_dev.h>     // Asserts

namespace Spark::UtilityBox::Logger {
    //------------------------------------------------------------------------------------------------------------------
    // LOG MESSAGE DATA
    //------------------------------------------------------------------------------------------------------------------
    class LogMessage::LogMessageData {
        public:
            /**
             * Allocates the necessary buffers to be able to process log messages.
             */
            explicit LogMessageData(LogMessageSeverity messageSeverity);

            /**
             * Cleans up information related to message processing.
             */
            ~LogMessageData();

            /**
             * Helper function to process printf-like message with variadic arguments. Emplaces processed message into
             * log record information store.
             * @param formatString - Printf-like format string for log messages.
             * @param argList      - Printf-like variadic arguments.
             * @return Pointer to the buffer which holds the processed message.
             */
            void ProcessMessage(const char* formatString, std::va_list argList);

            /**
             * Get a reference to the store of log record information this LogMessage instance holds.
             * @return Reference to LogMessage data.
             */
            _NODISCARD_ std::vector<LogRecord>& GetLogRecords();

            /**
             * Get the severity of this message.
             * @return Severity of this message.
             */
            _NODISCARD_ LogMessageSeverity& GetMessageSeverity();

            /**
             * OnUpdate the message severity. Can be used anytime before the message is logged.
             * @param messageSeverity - New message severity.
             */
            void SetMessageSeverity(LogMessageSeverity& messageSeverity);

        private:
            std::vector<LogRecord> _logMessages; // Store of log record information
            LogMessageSeverity _messageSeverity; // Message severity

            unsigned _processingBufferSize; // Size of the processing buffer.
            char* _processingBuffer;        // Pointer to the processing buffer.
    };

    // Allocates the necessary buffers to be able to process log messages.
    LogMessage::LogMessageData::LogMessageData(LogMessageSeverity messageSeverity) : _messageSeverity(messageSeverity),
                                                                                     _processingBufferSize(64u)
                                                                                     {
        // Start buffer with (baseline) 64 byte size.
        _processingBuffer = new (std::nothrow) char[_processingBufferSize];
        ASSERT(_processingBuffer != nullptr, "Operation new failed to allocate log message processing buffer - program is out of memory.");
    }

    // Cleans up information related to message processing.
    LogMessage::LogMessageData::~LogMessageData() {
        delete [] _processingBuffer;
    }

    // Get a reference to the store of log record information this LogMessage instance holds.
    std::vector<LogMessage::LogRecord>& LogMessage::LogMessageData::GetLogRecords() {
        return _logMessages;
    }

    // Get the severity of this message.
    LogMessageSeverity &LogMessage::LogMessageData::GetMessageSeverity() {
        return _messageSeverity;
    }

    // OnUpdate the message severity. Can be used anytime before the message is logged.
    void LogMessage::LogMessageData::SetMessageSeverity(LogMessageSeverity &messageSeverity) {
        _messageSeverity = messageSeverity;
    }


    //------------------------------------------------------------------------------------------------------------------
    // LOG MESSAGE
    //------------------------------------------------------------------------------------------------------------------
    // Construct an instance of a message to log.
    LogMessage::LogMessage(LogMessageSeverity messageSeverity) : _data(new LogMessageData(messageSeverity)) {
        // Nothing to do here.
    }

    // Log message destructor.
    LogMessage::~LogMessage() {
        delete _data;
    }

    // Get a reference to the store of log record information this LogMessage instance holds.
    std::vector<LogMessage::LogRecord>& LogMessage::GetLogRecords() {
        return _data->GetLogRecords();
    }

    // Get the severity of this message.
    LogMessageSeverity& LogMessage::GetMessageSeverity() {
        return _data->GetMessageSeverity();
    }

    // OnUpdate the message severity. Can be used anytime before the message is logged.
    void LogMessage::SetMessageSeverity(LogMessageSeverity messageSeverity) {
        _data->SetMessageSeverity(messageSeverity);
    }

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
        _data->ProcessMessage(formatString, args);
        va_end(args);
    }

    void LogMessage::Supply(const char *formatString, std::va_list argList) {
        _data->ProcessMessage(formatString, argList);
    }

    // Helper function to process printf-like message with variadic arguments.
    void LogMessage::LogMessageData::ProcessMessage(const char* formatString, std::va_list argList) {
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
        _logMessages.emplace_back(_processingBuffer, Timing::TimeStamp());
    }
}
