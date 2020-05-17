
#ifndef DATASTRUCTURES_LOGGER_CPP
#define DATASTRUCTURES_LOGGER_CPP

#include "logger.h"
#include <vector>

namespace UtilityBox::Logger {
    struct LogMessage::LogMessageData {
        explicit LogMessageData(LogMessageSeverity messageSeverity);
        ~LogMessageData();
        void ProcessMessage(const char* formatString, std::va_list argList);

        unsigned _processingBufferSize;
        char* _processingBuffer;
        LogMessageSeverity _severity;
        std::vector<std::string> _formattedMessages;
    };

    // LogMessage functions begin
    LogMessage::LogMessage(LogMessageSeverity messageSeverity) : _data(std::make_unique<LogMessageData>(messageSeverity)) {
    }

    LogMessage::LogMessage(LogMessageSeverity messageSeverity, const char* formatString, ...) : _data(std::make_unique<LogMessageData>(messageSeverity)){
        // _data is guaranteed to be initialized when the constructor body code is executed
        std::va_list args;
        va_start(args, formatString);
        _data->ProcessMessage(formatString, args);
        va_end(args);
    }

    LogMessage::~LogMessage() {
        _data.reset();
    }

    void LogMessage::Supply(const char* formatString, ...) {
        std::va_list args;
        va_start(args, formatString);
        _data->ProcessMessage(formatString, args);
        va_end(args);
    }
    // LogMessage functions end

    // LogMessageData functions begin
    LogMessage::LogMessageData::LogMessageData(LogMessageSeverity messageSeverity) : _processingBufferSize(64u), _severity(messageSeverity) {
        _processingBuffer = new (std::nothrow) char[_processingBufferSize];
        // TODO: assert
    }

    LogMessage::LogMessageData::~LogMessageData() {
        delete [] _processingBuffer;
        // TODO: _formattedMessages.clear();
    }

    void LogMessage::LogMessageData::ProcessMessage(const char* formatString, std::va_list argList) {
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

        _formattedMessages.emplace_back(_processingBuffer);
    }
    // LogMessageData functions end
}

#endif

//namespace UtilityBox {
//    namespace Logger {
//
//        typedef std::chrono::time_point<std::chrono::high_resolution_clock> timestampType;
//
//        struct LogMessageData {
//            LogMessageData() = default;
//            std::string _messageFormatted;      // Formatted log message
//            std::string _logNumberFormatted;    // Log number
//            std::string _calendarDateFormatted; // Calendar date and local time of this log
//            std::string _severityFormatted;     // Formatted log severity
//            std::string _timestampFormatted;    // Timestamp of this log since the log file was open
//
//            /**
//             * Print the formatted log to the given stream
//             * @param stream Stream to print to
//             * @param message Log message to print
//             * @return Stream that was printed to
//             */
//            friend std::ostream& operator<<(std::ostream &stream, const LogMessageData &message);
//        };
//
//        class LogMessageBackEnd {
//            public:
//                LogMessageBackEnd();
//                ~LogMessageBackEnd();
//                void ProcessMessage(LogMessageSeverity messageSeverity, timestampType timestamp, const char* formatString, std::va_list args);
//                void Clear();
//
//                friend std::ostream& operator<<(std::ostream& stream, const LogMessageBackEnd& storage);
//
//            private:
//                // formatting functions
//                _NODISCARD_ std::string FormatMessage(const char* formatString, std::va_list args);
//                _NODISCARD_ std::string FormatLogNumber();
//                _NODISCARD_ std::string FormatCalendarDate();
//                _NODISCARD_ std::string FormatMessageSeverity(LogMessageSeverity severity);
//                _NODISCARD_ std::string FormatTimestamp(timestampType timestamp);
//
//                std::stringstream _format;
//
//                char* _logBuffer;
//                char* _calendarBuffer;
//
//                unsigned _logBufferSize;
//                unsigned _calendarBufferSize;
//                std::time_t _calendarTime;
//                bool _isMultiLine;
//
//                std::vector<LogMessageData> _messages;
//        };
//
//        struct LogMessageBackEndHeader {
//            LogMessageBackEndHeader(bool inUse, LogMessageBackEnd* data);
//            bool _inUse;
//            LogMessageBackEnd* _data;
//        };
//
//        class LoggingSystem {
//            public:
//                _NODISCARD_ static LoggingSystem *GetInstance();
//                _NODISCARD_ unsigned ConstructMessageStorage();
//                _NODISCARD_ LogMessageBackEndHeader & GetBackEnd(unsigned ID);
//                _NODISCARD_ UtilityBox::Logger::LoggerConfiguration& GetConfig();
//                _NODISCARD_ unsigned GetLogCount();
//                _NODISCARD_ timestampType GetLogStartTime();
//
//                void Log(SingleLineMessage*);
//                void Log(MultiLineMessage*);
//                void Log(unsigned);
//
//            private:
//                // SINGLETON
//                static LoggingSystem* _loggingSystem;
//
//                // FUNCTIONS
//                LoggingSystem();
//                ~LoggingSystem();
//
//                // LoggingSystem data
//                UtilityBox::Logger::LoggerConfiguration _config;
//                timestampType _loggerStartTime;
//                std::ofstream _logger;
//                std::string _logFile;
//                unsigned _logCount;
//
//                std::vector<LogMessageBackEndHeader> _messages;
//
//        };
//
//        LogMessageBackEndHeader::LogMessageBackEndHeader(bool inUse, LogMessageBackEnd *data) : _inUse(inUse), _data(data) {
//        }
//
//        LoggingSystem* LoggingSystem::_loggingSystem = nullptr;
//
//        LoggingSystem *LoggingSystem::GetInstance() {
//            if (!_loggingSystem) {
//                _loggingSystem = new LoggingSystem();
//
//                // check the open status of the log file
//                _loggingSystem->_logger.open(_loggingSystem->_logFile, std::ios_base::in | std::ios_base::trunc);
//            }
//
//            return _loggingSystem;
//        }
//
//        void LoggingSystem::Log(SingleLineMessage* message) {
//            _logger << *message;
//        }
//
//        void LoggingSystem::Log(MultiLineMessage *message) {
//            _logger << *message;
//        }
//
//        void LoggingSystem::Log(unsigned ID) {
//            _logger << *(_messages.at(ID)._data);
//        }
//
//        LoggingSystem::LoggingSystem() {
//            _loggerStartTime = std::chrono::high_resolution_clock::now();
//            _logFile = "log2.txt";
//            _logCount = 0;
//        }
//
//        LoggingSystem::~LoggingSystem() {
//            _loggingSystem->_logger.close();
//        }
//
//        UtilityBox::Logger::LoggerConfiguration &LoggingSystem::GetConfig() {
//            return _config;
//        }
//
//        unsigned LoggingSystem::GetLogCount() {
//            return _logCount++;
//        }
//
//        timestampType LoggingSystem::GetLogStartTime() {
//            return _loggerStartTime;
//        }
//
//        unsigned LoggingSystem::ConstructMessageStorage() {
//            unsigned size = _messages.size();
//
//            // find an already existing header that has been cleared and is not in use
//            for (unsigned i = 0; i < size; ++i) {
//                auto& header = _messages.at(i);
//
//                if (!header._inUse) {
//                    header._inUse = true;
//                    return i;
//                }
//            }
//
//            // all headers are taken, create a new one
//            _messages.emplace_back(true, new LogMessageBackEnd());
//            return size;
//        }
//
//        LogMessageBackEndHeader& LoggingSystem::GetBackEnd(unsigned int ID) {
//            return _messages.at(ID);
//        }
//
//        std::ostream &operator<<(std::ostream &stream, const LogMessageData &message) {
//            const LoggerConfiguration& config = LoggingSystem::GetInstance()->GetConfig();
//
//            if (config.includeMessageNum) {
//                stream << message._logNumberFormatted.c_str() << " ";
//            }
//
//            if (config.includeDateTime) {
//                stream << message._calendarDateFormatted.c_str() << " ";
//            }
//
//            if (config.includeMessageTimestamp) {
//                stream << message._timestampFormatted.c_str() << " ";
//            }
//
//            if (config.includeMessageSeverity) {
//                stream << message._severityFormatted.c_str() << " ";
//            }
//
//            stream << message._messageFormatted.c_str() << std::endl;
//
//            return stream;
//        }
//
//        std::string LogMessageBackEnd::FormatLogNumber() {
//            unsigned logMessageID = LoggingSystem::GetInstance()->GetLogCount();
//            _format << "[";
//            _format << std::setfill('0') << std::setw(3);
//            _format << logMessageID / 1000;
//
//            _format << " ";
//
//            _format << std::setfill('0') << std::setw(3);
//            _format << logMessageID % 1000;
//            _format << "]";
//
//            std::string logNumberFormatted = _format.str();
//
//            // clear buffer
//            _format.str(std::string());
//
//            return logNumberFormatted;
//        }
//
//        std::string LogMessageBackEnd::FormatCalendarDate() {
//            while (!std::strftime(_calendarBuffer, _calendarBufferSize, UtilityBox::Logger::GetLoggerConfig().calendarHeaderFormat.c_str(), std::localtime(&_calendarTime))) {
//                _calendarBufferSize *= 2;
//                delete[] _calendarBuffer;
//                _calendarBuffer = new char[_calendarBufferSize];
//            }
//
//            return _calendarBuffer;
//        }
//
//        std::string LogMessageBackEnd::FormatMessageSeverity(LogMessageSeverity messageSeverity) {
//            _format << "[";
//
//            // [  DEBUG  ]
//            // [ WARNING ]
//            // [  ERROR  ]
//            // [ UNKNOWN ]
//            switch (messageSeverity) {
//                case DEBUG:
//                    _format << "  DEBUG  ";
//                    break;
//                case WARNING:
//                    _format << " WARNING ";
//                    break;
//                case ERROR:
//                    _format << "  ERROR  ";
//                    break;
//                default:
//                    _format << " UNKNOWN ";
//                    break;
//            }
//
//            _format << "]";
//            std::string severityFormatted = _format.str();
//
//            // clear buffer
//            _format.str(std::string());
//            return severityFormatted;
//        }
//
//        std::string LogMessageBackEnd::FormatTimestamp(timestampType timestamp) {
//            // get the time in milliseconds
//            unsigned long elapsed = (timestamp - LoggingSystem::GetInstance()->GetLogStartTime()).count() / 1000;
//
//            // milliseconds
//            unsigned long milliseconds = elapsed;
//            // seconds
//            unsigned long seconds = elapsed / 1000;
//            // minutes
//            unsigned long minutes = elapsed / 60000;
//
//            // 000m 00s 0000ms [SEVERITY] MESSAGE
//            // append minutes
//            _format << std::setfill('0') << std::setw(3);
//            _format << minutes << "m ";
//
//            // append seconds
//            _format << std::setfill('0') << std::setw(2);
//            _format << seconds << "s ";
//
//            // append milliseconds
//            _format << std::setfill('0') << std::setw(4);
//            _format << milliseconds << "ms ";
//
//            std::string timestampFormatted = _format.str();
//
//            // clear buffer
//            _format.str(std::string());
//            return timestampFormatted;
//        }
//
//        std::string LogMessageBackEnd::FormatMessage(const char *formatString, std::va_list args) {

//        }
//
//        LogMessageBackEnd::LogMessageBackEnd() : _logBufferSize(256u), _calendarBufferSize(64u), _calendarTime(std::time(nullptr)), _isMultiLine(false) {
//            // allocate buffers
//            _logBuffer = new(std::nothrow) char[_logBufferSize];
//            _calendarBuffer = new(std::nothrow) char[_calendarBufferSize];
//            //TODO: asserts for memory checks
//        }
//
//        LogMessageBackEnd::~LogMessageBackEnd() {
//            delete[] _logBuffer;
//            delete[] _calendarBuffer;
//        }
//
//        void LogMessageBackEnd::ProcessMessage(LogMessageSeverity messageSeverity, timestampType timestamp, const char* formatString, std::va_list args) {
//            _messages.emplace_back();
//            LogMessageData& data = _messages.at(_messages.size() - 1);
//            data._messageFormatted = FormatMessage(formatString, args);
//            data._logNumberFormatted = FormatLogNumber();
//            data._severityFormatted = FormatMessageSeverity(messageSeverity);
//            data._calendarDateFormatted = FormatCalendarDate();
//            data._timestampFormatted = FormatTimestamp(timestamp);
//        }
//
//        std::ostream &operator<<(std::ostream &stream, const LogMessageBackEnd &storage) {
//            const LoggerConfiguration& config = LoggingSystem::GetInstance()->GetConfig();
//
//            for (auto& message : storage._messages) {
//                stream << message;
//            }
//
//            if (config.separateMessages) {
//                stream << std::endl << std::endl;
//            }
//
//            return stream;
//        }
//
//        void LogMessageBackEnd::Clear() {
//            memset(_logBuffer, 0, _logBufferSize);
//            memset(_calendarBuffer, 0, _calendarBufferSize);
//            _messages.clear();
//            _format.str(std::string());
//        }
//    }
//}
//
//
//// PUBLIC API DEFINITIONS
//namespace UtilityBox {
//    namespace Logger {
//        // LoggingSystem functions
//        void Log(SingleLineMessage *msg) {
//            return LoggingSystem::GetInstance()->Log(msg);
//        }
//
//        void Log(MultiLineMessage *msg) {
//            return LoggingSystem::GetInstance()->Log(msg);
//        }
//
//        void Log(LogMessageSeverity messageSeverity, const char* formatString, ...) {
//            // record timestamp immediately
//            timestampType timeStamp = std::chrono::high_resolution_clock::now();
//
//            unsigned constructedID = LoggingSystem::GetInstance()->ConstructMessageStorage();
//            LogMessageBackEndHeader& storage = LoggingSystem::GetInstance()->GetBackEnd(constructedID);
//
//            std::va_list args;
//            va_start(args, formatString);
//            storage._data->ProcessMessage(messageSeverity, timeStamp, formatString, args);
//            va_end(args);
//
//            LoggingSystem::GetInstance()->Log(constructedID);
//
//            storage._inUse = false;
//            storage._data->Clear();
//        }
//
//        LoggerConfiguration &GetLoggerConfig() {
//            return LoggingSystem::GetInstance()->GetConfig();
//        }
//
//        // Single-line message functions
//        SingleLineMessage::~SingleLineMessage() = default;
//
//        SingleLineMessage::SingleLineMessage(LogMessageSeverity messageSeverity, const char *formatString, ...) {
//            // record timestamp immediately
//            timestampType timeStamp = std::chrono::high_resolution_clock::now();
//
//            _dataIndex = LoggingSystem::GetInstance()->ConstructMessageStorage();
//            LogMessageBackEndHeader& storage = LoggingSystem::GetInstance()->GetBackEnd(_dataIndex);
//
//            std::va_list args;
//            va_start(args, formatString);
//            storage._data->ProcessMessage(messageSeverity, timeStamp, formatString, args);
//            va_end(args);
//        }
//
//        std::ostream &operator<<(std::ostream &stream, const SingleLineMessage &msg) {
//            LogMessageBackEndHeader& storage = LoggingSystem::GetInstance()->GetBackEnd(msg._dataIndex);
//            stream << *(storage._data);
//            storage._inUse = false;
//            storage._data->Clear();
//            return stream;
//        }
//
//        unsigned SingleLineMessage::GetDataIndex() {
//            return _dataIndex;
//        }
//
//        // Multiple-line message functions
//        MultiLineMessage::MultiLineMessage() {
//            _dataIndex = LoggingSystem::GetInstance()->ConstructMessageStorage();
//        }
//
//        MultiLineMessage::MultiLineMessage(LogMessageSeverity messageSeverity, const char *formatString, ...) {
//            // record timestamp immediately
//            timestampType timeStamp = std::chrono::high_resolution_clock::now();
//
//            _dataIndex = LoggingSystem::GetInstance()->ConstructMessageStorage();
//            LogMessageBackEndHeader& storage = LoggingSystem::GetInstance()->GetBackEnd(_dataIndex);
//
//            std::va_list args;
//            va_start(args, formatString);
//            storage._data->ProcessMessage(messageSeverity, timeStamp, formatString, args);
//            va_end(args);
//        }
//
//        void MultiLineMessage::Supply(LogMessageSeverity messageSeverity, const char *formatString, ...) const {
//            // record timestamp immediately
//            timestampType timeStamp = std::chrono::high_resolution_clock::now();
//
//            LogMessageBackEndHeader& storage = LoggingSystem::GetInstance()->GetBackEnd(_dataIndex);
//
//            std::va_list args;
//            va_start(args, formatString);
//            storage._data->ProcessMessage(messageSeverity, timeStamp, formatString, args);
//            va_end(args);
//        }
//
//        MultiLineMessage::~MultiLineMessage() = default;
//
//        std::ostream &operator<<(std::ostream &stream, const MultiLineMessage &msg) {
//            LogMessageBackEndHeader& storage = LoggingSystem::GetInstance()->GetBackEnd(msg._dataIndex);
//            stream << *(storage._data);
//            storage._inUse = false;
//            storage._data->Clear();
//            return stream;
//        }
//
//        unsigned MultiLineMessage::GetDataIndex() {
//            return _dataIndex;
//        }
//    }
//}
//
//#endif
