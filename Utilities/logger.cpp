
#ifndef DATASTRUCTURES_LOGGER_CPP
#define DATASTRUCTURES_LOGGER_CPP

#include "logger.h"
#include "assert_dev.h"
#include <fstream>
#include <string>
#include <cstdarg>
#include <cstring>

// TODO templatize based on a policy
class _Logger {
    public:
        static _Logger *GetInstance();
        void SetLogFile(const std::string &logFile);

        void logMessage(const std::string &message, UtilityBox::Logger::MessageSeverity = UtilityBox::Logger::DEBUG);
        void logMessage(const char* formatString, va_list args, UtilityBox::Logger::MessageSeverity = UtilityBox::Logger::DEBUG);

    private:
        _Logger() = default;
        ~_Logger();

        static _Logger* _loggingSystem;
        std::fstream _logger;
        std::string _logFile = "log.txt";
};

_Logger* _Logger::_loggingSystem = nullptr;

_Logger *_Logger::GetInstance() {
    if (!_loggingSystem) {
        _loggingSystem = new _Logger();
        _loggingSystem->_logger.open(_loggingSystem->_logFile, std::ios_base::in | std::ios_base::trunc);
        ASSERT(ASSERT_LEVEL_FATAL, _loggingSystem->_logger.is_open(), "Logging system failed to open provided log file. %i", 0);
    }

    return _loggingSystem;
}

void _Logger::logMessage(const char *formatString, va_list args, UtilityBox::Logger::MessageSeverity level) {
    //fprintf(_logger., formatString, args);

    _logger << formatString << " logged with severity level: " << level << std::endl;
}


_Logger::~_Logger() {
    _loggingSystem->_logger.close();
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

namespace UtilityBox {
    namespace Logger {
        void logMessage(MessageSeverity severityLevel, const char* formatString, ...) {
            va_list args;
            va_start(args, formatString);
            _Logger::GetInstance()->logMessage(formatString, args);
            va_end(args);
        }
    }
}

#endif
