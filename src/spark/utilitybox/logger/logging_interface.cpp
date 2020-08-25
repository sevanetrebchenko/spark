
#include <spark/utilitybox/logger/logging_interface.h>    // ILoggable
#include <spark/utilitybox/logger/log_message_severity.h> // LogMessageSeverity
#include <spark/utilitybox/logger/logging_system.h>             // LoggingSystem

namespace Spark::UtilityBox::Logger {
    //------------------------------------------------------------------------------------------------------------------
    // ILOGGABLE DATA
    //------------------------------------------------------------------------------------------------------------------
    struct ILoggable::ILoggableData {
        explicit ILoggableData(const std::string& name);
        ~ILoggableData() = default;

        const char* _systemName;
        UtilityBox::Logger::LoggingSystem _loggingSystem { _systemName };
    };

    ILoggable::ILoggableData::ILoggableData(const std::string& name) : _systemName(name.c_str()) {
        // Nothing to do here.
    }


    //------------------------------------------------------------------------------------------------------------------
    // ILOGGABLE
    //------------------------------------------------------------------------------------------------------------------
    ILoggable::ILoggable(const std::string& systemName) : _data(new ILoggableData(systemName)) {
        // Nothing to do here.
    }

    void ILoggable::LogDebug(const char *formatString, ...) const {
        std::va_list argsList;
        va_start(argsList, formatString);
        _data->_loggingSystem.Log(LogMessageSeverity::DEBUG, formatString, argsList);
        va_end(argsList);
    }

    void ILoggable::LogWarning(const char *formatString, ...) const {
        std::va_list argsList;
        va_start(argsList, formatString);
        _data->_loggingSystem.Log(LogMessageSeverity::WARNING, formatString, argsList);
        va_end(argsList);
    }

    void ILoggable::LogError(const char *formatString, ...) const {
        std::va_list argsList;
        va_start(argsList, formatString);
        _data->_loggingSystem.Log(LogMessageSeverity::SEVERE, formatString, argsList);
        va_end(argsList);
    }

    ILoggable::~ILoggable() {
        delete _data;
    }

}
