
#include <utilitybox/logger/logging_interface.h>
#include <utilitybox/logger/logging_system.h>

namespace Spark::UtilityBox::Logger {
    //------------------------------------------------------------------------------------------------------------------
    // ILOGGABLE DATA
    //------------------------------------------------------------------------------------------------------------------
    struct ILoggable::ILoggableData {
        public:
            explicit ILoggableData(const char* systemName);
            ~ILoggableData() = default;

            const char* _systemName;
            UtilityBox::Logger::LoggingSystem _loggingSystem { _systemName };
    };

    ILoggable::ILoggableData::ILoggableData(const char *systemName) : _systemName(systemName) {
        // Nothing to do here.
    }


    //------------------------------------------------------------------------------------------------------------------
    // ILOGGABLE
    //------------------------------------------------------------------------------------------------------------------
    ILoggable::ILoggable(const char *systemName) : _data(new ILoggableData(systemName)) {
        // Nothing to do here.
    }

    ILoggable::ILoggable(const std::string& systemName) : _data(new ILoggableData(systemName.c_str())){

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
