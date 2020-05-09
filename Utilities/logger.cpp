
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

class _Logger {
    public:
        static _Logger *GetInstance();

        void SetLogFile(const std::string &logFile);
        void logMessage(const char* formatString, va_list args, UtilityBox::Logger::MessageSeverity = UtilityBox::Logger::DEBUG);

    private:
        // SINGLETON
        static _Logger* _loggingSystem;

        // FUNCTIONS
        _Logger();
        ~_Logger();
        std::string GetTimestamp();
        std::string GetSeverity(UtilityBox::Logger::MessageSeverity);

        // DATA
        std::chrono::time_point<std::chrono::system_clock> _startTime;
        std::ostringstream _format;
        std::ofstream _logger;
        std::string _logFile;
        char* _buffer;
        unsigned _bufferSize;
        UtilityBox::Logger::LoggerLevel _level;
};

_Logger* _Logger::_loggingSystem = nullptr;

_Logger *_Logger::GetInstance() {
    if (!_loggingSystem) {
        _loggingSystem = new _Logger();

        // check the open status of the log file
        _loggingSystem->_logger.open(_loggingSystem->_logFile, std::ios_base::in | std::ios_base::trunc);
        ASSERT(ASSERT_LEVEL_FATAL, _loggingSystem->_logger.is_open(), "Logging system failed to open provided log file. Supplied log file name: %s", _loggingSystem->_logFile.c_str());
        UtilityBox::Logger::LogMessage(UtilityBox::Logger::DEBUG, "File \"log.txt\" successfully opened.");

        // check the allocation status of the buffer (marked as nothrow)
        ASSERT(ASSERT_LEVEL_FATAL, _loggingSystem->_buffer != nullptr, "Logger failed to allocate buffer.");
        UtilityBox::Logger::LogMessage(UtilityBox::Logger::DEBUG, "Logging buffer successfully allocated with size: %i", _loggingSystem->_bufferSize);
    }

    return _loggingSystem;
}

void _Logger::logMessage(const char *formatString, std::va_list args, UtilityBox::Logger::MessageSeverity severity) {
    // buffer could fail to allocate - this is only here to log the error message from failing to allocate the buffer
    if (_buffer) {
        int writeResult = vsnprintf(_loggingSystem->_buffer, _bufferSize, formatString, args);
        ASSERT(ASSERT_LEVEL_WARNING, writeResult > _bufferSize - 1,
               "Buffer write limit was reached. Supplied log message may have been truncated. Increase buffer limit. Characters written: %i. Buffer size: %i.",
               writeResult, _bufferSize);
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
            _logger << GetTimestamp() << GetSeverity(severity) << _buffer << std::endl;
            _logger.flush();
        }
    }
    else {
        _logger << GetTimestamp() << GetSeverity(severity) << formatString << std::endl;
        _logger.flush();
    }
}


_Logger::~_Logger() {
    _loggingSystem->_logger.close();
    delete[] _buffer;
}

void _Logger::SetLogFile(const std::string &logFile) {
    if (!strcmp(_loggingSystem->_logFile.c_str(), logFile.c_str())) {
        // flush and close previous file
        _loggingSystem->_logger.flush();
        _loggingSystem->_logger.close();

        // open new file and set up logger
        _loggingSystem->_logFile = logFile;
    }
}

_Logger::_Logger() {
    _logFile = "log.txt";
    _bufferSize = 1024;
    _level = UtilityBox::Logger::ALL;
    _buffer = new (std::nothrow) char[_bufferSize];
    _startTime = std::chrono::high_resolution_clock::now();
}

std::string _Logger::GetTimestamp() {
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

std::string _Logger::GetSeverity(UtilityBox::Logger::MessageSeverity severity) {
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

    _format << "]   ";
    std::string messageSeverity = _format.str();
    _format.str(std::string());

    return messageSeverity;
}

// PUBLIC API DEFINITIONS
namespace UtilityBox {
    namespace Logger {
        void LogMessage(MessageSeverity severityLevel, const char* formatString, ...) {
            va_list args;
            va_start(args, formatString);
            _Logger::GetInstance()->logMessage(formatString, args, severityLevel);
            va_end(args);
        }
    }
}

#endif
