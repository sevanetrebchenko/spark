
#ifndef DATASTRUCTURES_LOGGER_CPP
#define DATASTRUCTURES_LOGGER_CPP

#include <sstream>
#include <variant>
#include "logger.h"

namespace UtilityBox {
    namespace Logger {

        typedef std::chrono::time_point<std::chrono::high_resolution_clock> timestampType;

        struct LogMessageData {
            LogMessageData() = default;
            std::string _messageFormatted;
            std::string _logNumberFormatted;
            std::string _calendarDateFormatted;
            std::string _severityFormatted;
            std::string _timestampFormatted;

            friend std::ostream& operator<<(std::ostream &stream, const LogMessageData &message);
        };

        class LogMessageBackEnd {
            public:
                LogMessageBackEnd();
                ~LogMessageBackEnd();
                void ProcessMessage(LogMessageSeverity messageSeverity, timestampType timestamp, const char* formatString, std::va_list args);
                void Clear();

                friend std::ostream& operator<<(std::ostream& stream, const LogMessageBackEnd& storage);

            private:
                // formatting functions
                std::string FormatMessage(const char* formatString, std::va_list args);
                std::string FormatLogNumber();
                std::string FormatCalendarDate();
                std::string FormatMessageSeverity(LogMessageSeverity severity);
                std::string FormatTimestamp(timestampType timestamp);

                std::stringstream _format;

                char* _logBuffer;
                char* _calendarBuffer;

                unsigned _logBufferSize;
                unsigned _calendarBufferSize;
                std::time_t _calendarTime;

                std::vector<LogMessageData*> _messages;
        };

        class LoggingSystem {
            public:
                __NO_DISCARD__ static LoggingSystem *GetInstance();

                __NO_DISCARD__ unsigned ConstructMessageStorage();
                __NO_DISCARD__ std::pair<bool, LogMessageBackEnd*>& GetBackEnd(unsigned ID);

                __NO_DISCARD__ UtilityBox::Logger::LoggerConfiguration& GetConfig();
                __NO_DISCARD__ unsigned GetLogCount();
                __NO_DISCARD__ timestampType GetLogStartTime();

                void Log(SingleLineMessage*);
                void Log(MultiLineMessage*);
                void Log(unsigned);

            private:
                // SINGLETON
                static LoggingSystem* _loggingSystem;

                // FUNCTIONS
                LoggingSystem();
                ~LoggingSystem();

                // LoggingSystem data
                UtilityBox::Logger::LoggerConfiguration _config;
                timestampType _loggerStartTime;
                std::ofstream _logger;
                std::string _logFile;
                unsigned _logCount;

                std::vector<std::pair<bool, LogMessageBackEnd*>> _messages;
        };

        LoggingSystem* LoggingSystem::_loggingSystem = nullptr;

        LoggingSystem *LoggingSystem::GetInstance() {
            if (!_loggingSystem) {
                _loggingSystem = new LoggingSystem();

                // check the open status of the log file
                _loggingSystem->_logger.open(_loggingSystem->_logFile, std::ios_base::in | std::ios_base::trunc);
//        ASSERT(ASSERT_LEVEL_FATAL, _loggingSystem->_logger.is_open(), "Logging system failed to open provided log file. Supplied log file name: %s", _loggingSystem->_logFile.c_str());
//        UtilityBox::LoggingSystem::LogMessage(UtilityBox::LoggingSystem::DEBUG, "File \"log.txt\" successfully opened.");

                // check the allocation status of the buffer (marked as nothrow)
//        ASSERT(ASSERT_LEVEL_FATAL, _loggingSystem->_logBuffer != nullptr, "LoggingSystem failed to allocate buffer.");
//        UtilityBox::LoggingSystem::LogMessage(UtilityBox::LoggingSystem::DEBUG, "Logging buffer successfully allocated with size: %i", _loggingSystem->_logBufferSize);
            }

            return _loggingSystem;
        }

        void LoggingSystem::Log(SingleLineMessage* message) {
            // buffer could fail to allocate - this is only here to log the error message from failing to allocate the buffer
            _logger << *message;
//        int writeResult = vsnprintf(_loggingSystem->_logBuffer, _logBufferSize, formatString, args);
//        ASSERT(ASSERT_LEVEL_WARNING, writeResult > _logBufferSize - 1,
//               "Buffer write limit was reached. Supplied log message may have been truncated. Use UtilityBox::LoggingSystem::UpdateBufferSize() to increase the buffer limit. Characters written: %i. Current buffer size: %i.",
//               writeResult, _logBufferSize);
//        bool log = false;
//
//        switch (_level) {
//            case UtilityBox::LoggingSystem::ALL:
//                log = true;
//                break;
//            case UtilityBox::LoggingSystem::VERBOSE:
//                if (severity == UtilityBox::LoggingSystem::WARNING || severity == UtilityBox::LoggingSystem::ERROR) {
//                    log = true;
//                }
//                break;
//            case UtilityBox::LoggingSystem::MINIMAL:
//                if (severity == UtilityBox::LoggingSystem::ERROR) {
//                    log = true;
//                }
//                break;
//
//            default:
//                UtilityBox::LoggingSystem::LogMessage(UtilityBox::LoggingSystem::ERROR,"Logged message with unknown or invalid log level.");
//                break;
//        }
//
//        if (log) {
//            _logger << PrintLogCount() << " " << PrintDate() << " " << PrintLogTimestamp() << PrintLogSeverity(severity) << "    " << _logBuffer << std::endl;
//            _logger.flush();
//        }
//    }
        }

        void LoggingSystem::Log(MultiLineMessage *message) {
            _logger << *message;
        }

        void LoggingSystem::Log(unsigned ID) {
            _logger << *(_messages.at(ID).second);
        }

        LoggingSystem::LoggingSystem() {
            _loggerStartTime = std::chrono::high_resolution_clock::now();
            _logFile = "log2.txt";
            _logCount = 0;
        }

        LoggingSystem::~LoggingSystem() {
            _loggingSystem->_logger.close();
        }

        UtilityBox::Logger::LoggerConfiguration &LoggingSystem::GetConfig() {
            return _config;
        }

        unsigned LoggingSystem::GetLogCount() {
            return _logCount++;
        }

        timestampType LoggingSystem::GetLogStartTime() {
            return _loggerStartTime;
        }

        unsigned LoggingSystem::ConstructMessageStorage() {
            unsigned size = _messages.size();
            for (unsigned i = 0; i < size; ++i) {
                auto& pair = _messages.at(i);

                if (pair.first) {
                    pair.first = true;
                    return i;
                }
            }

            _messages.emplace_back(std::make_pair(true, new LogMessageBackEnd()));
            return size;
        }

        std::pair<bool, LogMessageBackEnd*>& LoggingSystem::GetBackEnd(unsigned int ID) {
            return _messages.at(ID);
        }

        std::ostream &operator<<(std::ostream &stream, const LogMessageData &message) {
            const LoggerConfiguration& config = LoggingSystem::GetInstance()->GetConfig();

            if (config.includeMessageNum) {
                stream << message._logNumberFormatted.c_str() << " ";
            }

            if (config.includeDateTime) {
                stream << message._calendarDateFormatted.c_str() << " ";
            }

            if (config.includeMessageTimestamp) {
                stream << message._timestampFormatted.c_str() << " ";
            }

            if (config.includeMessageSeverity) {
                stream << message._severityFormatted.c_str() << " ";
            }

            stream << message._messageFormatted.c_str() << std::endl;

            return stream;
        }

        std::string LogMessageBackEnd::FormatLogNumber() {
            unsigned logMessageID = LoggingSystem::GetInstance()->GetLogCount();
            _format << "[";
            _format << std::setfill('0') << std::setw(3);
            _format << logMessageID / 1000;

            _format << " ";

            _format << std::setfill('0') << std::setw(3);
            _format << logMessageID % 1000;
            _format << "]";

            std::string logNumberFormatted = _format.str();

            // clear buffer
            _format.str(std::string());

            return logNumberFormatted;
        }

        std::string LogMessageBackEnd::FormatCalendarDate() {
            while (!std::strftime(_calendarBuffer, _calendarBufferSize, UtilityBox::Logger::GetLoggerConfig().calendarHeaderFormat.c_str(), std::localtime(&_calendarTime))) {
                _calendarBufferSize *= 2;
                delete[] _calendarBuffer;
                _calendarBuffer = new char[_calendarBufferSize];
            }

            return _calendarBuffer;
        }

        std::string LogMessageBackEnd::FormatMessageSeverity(LogMessageSeverity messageSeverity) {
            _format << "[";

            // [  DEBUG  ]
            // [ WARNING ]
            // [  ERROR  ]
            // [ UNKNOWN ]
            switch (messageSeverity) {
                case DEBUG:
                    _format << "  DEBUG  ";
                    break;
                case WARNING:
                    _format << " WARNING ";
                    break;
                case ERROR:
                    _format << "  ERROR  ";
                    break;
                default:
                    _format << " UNKNOWN ";
                    break;
            }

            _format << "]";
            std::string severityFormatted = _format.str();

            // clear buffer
            _format.str(std::string());
            return severityFormatted;
        }

        std::string LogMessageBackEnd::FormatTimestamp(timestampType timestamp) {
            // get the time in milliseconds
            unsigned long elapsed = (timestamp - LoggingSystem::GetInstance()->GetLogStartTime()).count() / 1000;

            // milliseconds
            unsigned long milliseconds = elapsed;
            // seconds
            unsigned long seconds = elapsed / 1000;
            // minutes
            unsigned long minutes = elapsed / 60000;

            // 000m 00s 0000ms [SEVERITY] MESSAGE
            // append minutes
            _format << std::setfill('0') << std::setw(3);
            _format << minutes << "m ";

            // append seconds
            _format << std::setfill('0') << std::setw(2);
            _format << seconds << "s ";

            // append milliseconds
            _format << std::setfill('0') << std::setw(4);
            _format << milliseconds << "ms ";

            std::string timestampFormatted = _format.str();

            // clear buffer
            _format.str(std::string());
            return timestampFormatted;
        }

        std::string LogMessageBackEnd::FormatMessage(const char *formatString, std::va_list args) {
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

            return _logBuffer;
        }

        LogMessageBackEnd::LogMessageBackEnd() : _logBufferSize(256u), _calendarBufferSize(64u), _calendarTime(std::time(nullptr)) {
            // allocate buffers
            _logBuffer = new(std::nothrow) char[_logBufferSize];
            _calendarBuffer = new(std::nothrow) char[_calendarBufferSize];
            //TODO: asserts for memory checks
        }

        LogMessageBackEnd::~LogMessageBackEnd() {
            delete[] _logBuffer;
            delete[] _calendarBuffer;
        }

        void LogMessageBackEnd::ProcessMessage(LogMessageSeverity messageSeverity, timestampType timestamp, const char* formatString, std::va_list args) {
            auto* messageData = new LogMessageData();
            messageData->_messageFormatted = FormatMessage(formatString, args);
            messageData->_logNumberFormatted = FormatLogNumber();
            messageData->_severityFormatted = FormatMessageSeverity(messageSeverity);
            messageData->_calendarDateFormatted = FormatCalendarDate();
            messageData->_timestampFormatted = FormatTimestamp(timestamp);
            _messages.emplace_back(messageData);
        }

        std::ostream &operator<<(std::ostream &stream, const LogMessageBackEnd &storage) {
            const LoggerConfiguration& config = LoggingSystem::GetInstance()->GetConfig();

            for (auto& message : storage._messages) {
                stream << *message;
            }

            if (config.separateMessages) {
                stream << std::endl << std::endl;
            }

            return stream;
        }

        void LogMessageBackEnd::Clear() {
            memset(_logBuffer, 0, _logBufferSize);
            memset(_calendarBuffer, 0, _calendarBufferSize);
            _messages.clear();
            _format.str(std::string());
        }
    }
}


// PUBLIC API DEFINITIONS
namespace UtilityBox {
    namespace Logger {
        // LoggingSystem functions
        void Log(SingleLineMessage *msg) {
            return LoggingSystem::GetInstance()->Log(msg);
        }

        void Log(MultiLineMessage *msg) {
            return LoggingSystem::GetInstance()->Log(msg);
        }

        void Log(LogMessageSeverity messageSeverity, const char* formatString, ...) {
            // record timestamp immediately
            timestampType timeStamp = std::chrono::high_resolution_clock::now();

            unsigned constructedID = LoggingSystem::GetInstance()->ConstructMessageStorage();
            std::pair<bool, LogMessageBackEnd*>& storage = LoggingSystem::GetInstance()->GetBackEnd(constructedID);

            std::va_list args;
            va_start(args, formatString);
            storage.second->ProcessMessage(messageSeverity, timeStamp, formatString, args);
            va_end(args);

            LoggingSystem::GetInstance()->Log(constructedID);

            storage.first = false;
            storage.second->Clear();
        }

        LoggerConfiguration &GetLoggerConfig() {
            return LoggingSystem::GetInstance()->GetConfig();
        }

        // Single-line message functions
        SingleLineMessage::~SingleLineMessage() = default;

        SingleLineMessage::SingleLineMessage(LogMessageSeverity messageSeverity, const char *formatString, ...) {
            // record timestamp immediately
            timestampType timeStamp = std::chrono::high_resolution_clock::now();

            _dataIndex = LoggingSystem::GetInstance()->ConstructMessageStorage();
            std::pair<bool, LogMessageBackEnd*>& storage = LoggingSystem::GetInstance()->GetBackEnd(_dataIndex);

            std::va_list args;
            va_start(args, formatString);
            storage.second->ProcessMessage(messageSeverity, timeStamp, formatString, args);
            va_end(args);
        }

        std::ostream &operator<<(std::ostream &stream, const SingleLineMessage &msg) {
            std::pair<bool, LogMessageBackEnd*>& storage = LoggingSystem::GetInstance()->GetBackEnd(msg._dataIndex);
            stream << *(storage.second);
            storage.first = false;
            storage.second->Clear();
            return stream;
        }

        // Multiple-line message functions
        MultiLineMessage::MultiLineMessage() {
            _dataIndex = LoggingSystem::GetInstance()->ConstructMessageStorage();
        }

        MultiLineMessage::MultiLineMessage(LogMessageSeverity messageSeverity, const char *formatString, ...) {
            // record timestamp immediately
            timestampType timeStamp = std::chrono::high_resolution_clock::now();

            _dataIndex = LoggingSystem::GetInstance()->ConstructMessageStorage();
            std::pair<bool, LogMessageBackEnd*>& storage = LoggingSystem::GetInstance()->GetBackEnd(_dataIndex);

            std::va_list args;
            va_start(args, formatString);
            storage.second->ProcessMessage(messageSeverity, timeStamp, formatString, args);
            va_end(args);
        }

        void MultiLineMessage::Supply(LogMessageSeverity messageSeverity, const char *formatString, ...) const {
            // record timestamp immediately
            timestampType timeStamp = std::chrono::high_resolution_clock::now();

            std::pair<bool, LogMessageBackEnd*>& storage = LoggingSystem::GetInstance()->GetBackEnd(_dataIndex);

            std::va_list args;
            va_start(args, formatString);
            storage.second->ProcessMessage(messageSeverity, timeStamp, formatString, args);
            va_end(args);
        }

        MultiLineMessage::~MultiLineMessage() = default;

        std::ostream &operator<<(std::ostream &stream, const MultiLineMessage &msg) {
            std::pair<bool, LogMessageBackEnd*>& storage = LoggingSystem::GetInstance()->GetBackEnd(msg._dataIndex);
            stream << *(storage.second);
            storage.first = false;
            storage.second->Clear();
            return stream;
        }
    }
}

#endif
