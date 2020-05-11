
#ifndef DATASTRUCTURES_LOGGER_CPP
#define DATASTRUCTURES_LOGGER_CPP

#include "logger.h"
#include "assert_dev.h"
#include <fstream>
#include <cstdarg>
#include <cstring>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <vector>

class _Logger {
    public:
        static _Logger *GetInstance();

        void SetLogFile(const std::string &logFile);
        void LogMessage(UtilityBox::Logger::MessageSeverity severity, const char* formatString, va_list args);

        // support for block messages
        unsigned GetOpenBlockID();
        void StartBlockMessage(unsigned blockID);
        void LogBlockMessage(unsigned int blockID, UtilityBox::Logger::MessageSeverity, const char *formatString, std::va_list args);
        void EndBlockMessage(unsigned blockID);

    private:
        // SINGLETON
        static _Logger* _loggingSystem;

        // FUNCTIONS
        _Logger();
        ~_Logger();

        // Log message formatting
        std::string PrintLogTimestamp();
        std::string PrintLogSeverity(UtilityBox::Logger::MessageSeverity);
        std::string PrintLogCount();
        std::string PrintDate();

        // Logger data
        std::chrono::time_point<std::chrono::system_clock> _startTime;
        std::ostringstream _format;
        std::ofstream _logger;
        std::string _logFile;
        unsigned _logBufferSize;
        unsigned _calendarBufferSize;
        unsigned _logCount;
        unsigned _logHeaderLength;
        UtilityBox::Logger::LoggerLevel _level;
        std::time_t _calendarTime;
        char* _logBuffer;
        char* _calendarBuffer;

        // Block message data
        struct _blockMessageHeader {
            _blockMessageHeader();
            ~_blockMessageHeader();

            bool _inUse;
            std::vector<std::string> _data;
        };
        std::vector<_blockMessageHeader> _blockMessageStorage;
};

_Logger::_blockMessageHeader::_blockMessageHeader() : _inUse(false) {
}

_Logger::_blockMessageHeader::~_blockMessageHeader() {
    _inUse = false;
    _data.clear();
}

_Logger* _Logger::_loggingSystem = nullptr;

_Logger *_Logger::GetInstance() {
    if (!_loggingSystem) {
        _loggingSystem = new _Logger();

        // check the open status of the log file
        _loggingSystem->_logger.open(_loggingSystem->_logFile, std::ios_base::in | std::ios_base::trunc);
        ASSERT(ASSERT_LEVEL_FATAL, _loggingSystem->_logger.is_open(), "Logging system failed to open provided log file. Supplied log file name: %s", _loggingSystem->_logFile.c_str());
        UtilityBox::Logger::LogMessage(UtilityBox::Logger::DEBUG, "File \"log.txt\" successfully opened.");

        // check the allocation status of the buffer (marked as nothrow)
        ASSERT(ASSERT_LEVEL_FATAL, _loggingSystem->_logBuffer != nullptr, "Logger failed to allocate buffer.");
        UtilityBox::Logger::LogMessage(UtilityBox::Logger::DEBUG, "Logging buffer successfully allocated with size: %i", _loggingSystem->_logBufferSize);
    }

    return _loggingSystem;
}

void _Logger::LogMessage(UtilityBox::Logger::MessageSeverity severity, const char* formatString, va_list args) {
    // buffer could fail to allocate - this is only here to log the error message from failing to allocate the buffer
    if (_logBuffer) {
        int writeResult = vsnprintf(_loggingSystem->_logBuffer, _logBufferSize, formatString, args);
        ASSERT(ASSERT_LEVEL_WARNING, writeResult > _logBufferSize - 1,
               "Buffer write limit was reached. Supplied log message may have been truncated. Use UtilityBox::Logger::UpdateBufferSize() to increase the buffer limit. Characters written: %i. Current buffer size: %i.",
               writeResult, _logBufferSize);
        bool log = false;

        switch (_level) {
            case UtilityBox::Logger::ALL:
                log = true;
                break;
            case UtilityBox::Logger::VERBOSE:
                if (severity == UtilityBox::Logger::WARNING || severity == UtilityBox::Logger::ERROR) {
                    log = true;
                }
                break;
            case UtilityBox::Logger::MINIMAL:
                if (severity == UtilityBox::Logger::ERROR) {
                    log = true;
                }
                break;

            default:
                UtilityBox::Logger::LogMessage(UtilityBox::Logger::ERROR,"Logged message with unknown or invalid log level.");
                break;
        }

        if (log) {
            _logger << PrintLogCount() << " " << PrintDate() << " " << PrintLogTimestamp() << PrintLogSeverity(severity) << "    " << _logBuffer << std::endl;
            _logger.flush();
        }
    }
    else {
        _logger << PrintLogCount() << " " << PrintDate() << " " << PrintLogTimestamp() << PrintLogSeverity(severity) << "      " << formatString << std::endl;
        _logger.flush();
    }

    ++_logCount;
}


void _Logger::SetLogFile(const std::string &logFile) {
    if (!strcmp(_loggingSystem->_logFile.c_str(), logFile.c_str())) {
        // flush and close previous file
        _logger.flush();
        _logger.close();

        // open new file and set up logger
        _loggingSystem->_logFile = logFile;
        // check the open status of the log file
        _loggingSystem->_logger.open(_loggingSystem->_logFile, std::ios_base::in | std::ios_base::trunc);
        ASSERT(ASSERT_LEVEL_FATAL, _loggingSystem->_logger.is_open(), "Logging system failed to open provided log file. Supplied log file name: %s", _loggingSystem->_logFile.c_str());
        UtilityBox::Logger::LogMessage(UtilityBox::Logger::DEBUG, "File \"%s\" successfully opened.", _logFile.c_str());
    }
}

_Logger::_Logger() {
    _logFile = "log.txt";
    _logBufferSize = 1024;
    _calendarBufferSize = 64;
    _level = UtilityBox::Logger::ALL;
    _logBuffer = new (std::nothrow) char[_logBufferSize];
    _calendarBuffer = new (std::nothrow) char[_calendarBufferSize];
    _startTime = std::chrono::high_resolution_clock::now();
    _calendarTime = std::time(nullptr);
    _logCount = 0;
}

_Logger::~_Logger() {
    _loggingSystem->_logger.close();
    delete[] _logBuffer;
}

std::string _Logger::PrintLogCount() {
    _format << "[";
    _format << std::setfill('0') << std::setw(3);
    _format << _logCount / 1000;

    _format << " ";

    _format << std::setfill('0') << std::setw(3);
    _format << _logCount % 1000;
    _format << "]";

    std::string formattedLog = _format.str();
    _format.str(std::string()); // clear buffer

    return formattedLog;
}

std::string _Logger::PrintDate() {
    // longest possible string with given setup is 42 characters - [WEDNESDAY 00, SEPTEMBER 0000 - 00:00::00]
    if (std::strftime(_calendarBuffer, _calendarBufferSize, "[%A %d, %B %Y - %H:%M:%S] ", std::localtime(&_calendarTime))) {
        _format << _calendarBuffer;
    }

    std::string formattedDate = _format.str();
    _format.str(std::string()); // clear buffer

    return formattedDate;
}

std::string _Logger::PrintLogTimestamp() {
    // time since logger was initialized
    std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::high_resolution_clock::now();

    // get the time in milliseconds
    unsigned long elapsed = (currentTime - _startTime).count() / 1000;

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

    std::string currentTimeStamp = _format.str();
    _format.str(std::string()); // clear buffer

    return currentTimeStamp;
}

std::string _Logger::PrintLogSeverity(UtilityBox::Logger::MessageSeverity severity) {
    _format << "[";

    // [  DEBUG  ]
    // [ WARNING ]
    // [  ERROR  ]
    // [ UNKNOWN ]
    switch (severity) {
        case UtilityBox::Logger::DEBUG:
            _format << "  DEBUG  ";
            break;
        case UtilityBox::Logger::WARNING:
            _format << " WARNING ";
            break;
        case UtilityBox::Logger::ERROR:
            _format << "  ERROR  ";
            break;
        default:
            _format << " UNKNOWN ";
            break;
    }

    _format << "]";
    std::string messageSeverity = _format.str();
    _format.str(std::string());

    return messageSeverity;
}

unsigned _Logger::GetOpenBlockID() {
    if (_blockMessageStorage.empty()) {
        _blockMessageStorage.emplace_back();
        return 0;
    }
    else {
        for (int i = 0; i < _blockMessageStorage.size(); ++i) {
            if (!_blockMessageStorage.at(i)._inUse) {
                return i;
            }
        }
    }

    _blockMessageStorage.emplace_back();
    return _blockMessageStorage.size();
}

void _Logger::StartBlockMessage(unsigned int blockID) {
    _blockMessageStorage[blockID]._inUse = true;
}

void _Logger::LogBlockMessage(unsigned int blockID, UtilityBox::Logger::MessageSeverity, const char *formatString, std::va_list args) {
    // buffer could fail to allocate - this is only here to log the error message from failing to allocate the buffer
    int writeResult = vsnprintf(_logBuffer, _logBufferSize, formatString, args);
//    ASSERT(ASSERT_LEVEL_WARNING, writeResult > _logBufferSize - 1,
//           "Buffer write limit was reached. Supplied log message may have been truncated. Use UtilityBox::Logger::UpdateBufferSize() to increase the buffer limit. Characters written: %i. Current buffer size: %i.",
//           writeResult, _logBufferSize);

    _blockMessageStorage[blockID]._data.emplace_back(_logBuffer);
}

void _Logger::EndBlockMessage(unsigned int blockID) {
    for (auto& string : _blockMessageStorage[blockID]._data) {
        _logger << string << std::endl;
    }

    _blockMessageStorage[blockID]._data.clear();
    _blockMessageStorage[blockID]._inUse = false;
}


// PUBLIC API DEFINITIONS
namespace UtilityBox {
    namespace Logger {
        // single-line messages
        void LogMessage(MessageSeverity severityLevel, const char* formatString, ...) {
            va_list args;
            va_start(args, formatString);
            _Logger::GetInstance()->LogMessage(severityLevel, formatString, args);
            va_end(args);
        }

        // block messages
        unsigned GetOpenBlockID() {
            return _Logger::GetInstance()->GetOpenBlockID();
        }

        void StartMessageBlock(unsigned blockID) {
            _Logger::GetInstance()->StartBlockMessage(blockID);
        }

        void LogBlockMessage(unsigned blockID, MessageSeverity severityLevel, const char* formatString, ...) {
            va_list args;
            va_start(args, formatString);
            _Logger::GetInstance()->LogBlockMessage(blockID, severityLevel, formatString, args);
            va_end(args);
        }

        void EndMessageBlock(unsigned blockID) {
            _Logger::GetInstance()->EndBlockMessage(blockID);
        }

        // updating file
        void SetLogFile(const std::string &newFilename) {
            _Logger::GetInstance()->SetLogFile(newFilename);
        }
    }
}

#endif
