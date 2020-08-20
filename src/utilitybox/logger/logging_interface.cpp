
#include <utilitybox/logger/logging_interface.h>
#include <utilitybox/logger/logging_system.h>

namespace Spark::UtilityBox::Logger {
    //------------------------------------------------------------------------------------------------------------------
    // ILOGGABLE DATA
    //------------------------------------------------------------------------------------------------------------------
    class ILoggable::ILoggableData {
        public:
            explicit ILoggableData(const char* systemName);
            ~ILoggableData() = default;

            void LogDebug(const char* formatString, std::va_list argsList);
            void LogWarning(const char* formatString, std::va_list argsList);
            void LogError(const char* formatString, std::va_list argsList);

        private:
            const char* _systemName;
            UtilityBox::Logger::LoggingSystem _loggingSystem { _systemName };
    };

    ILoggable::ILoggableData::ILoggableData(const char *systemName) : _systemName(systemName) {
        // Nothing to do here.
    }

    void ILoggable::ILoggableData::LogDebug(const char* formatString, std::va_list argsList) {
        _loggingSystem.Log(LogMessageSeverity::DEBUG, formatString, argsList);
    }

    void ILoggable::ILoggableData::LogWarning(const char* formatString, std::va_list argsList) {
        _loggingSystem.Log(LogMessageSeverity::WARNING, formatString, argsList);
    }

    void ILoggable::ILoggableData::LogError(const char* formatString, std::va_list argsList) {
        _loggingSystem.Log(LogMessageSeverity::SEVERE, formatString, argsList);
    }


    //------------------------------------------------------------------------------------------------------------------
    // ILOGGABLE
    //------------------------------------------------------------------------------------------------------------------
    ILoggable::ILoggable(const char *systemName) : _data(new ILoggableData(systemName)) {
        // Nothing to do here.
    }

    void ILoggable::LogDebug(const char *formatString, ...) {
        std::va_list argsList;
        va_start(argsList, formatString);
        _data->LogDebug(formatString, argsList);
        va_end(argsList);
    }

    void ILoggable::LogWarning(const char *formatString, ...) {
        std::va_list argsList;
        va_start(argsList, formatString);
        _data->LogWarning(formatString, argsList);
        va_end(argsList);
    }

    void ILoggable::LogError(const char *formatString, ...) {
        std::va_list argsList;
        va_start(argsList, formatString);
        _data->LogError(formatString, argsList);
        va_end(argsList);
    }

    ILoggable::~ILoggable() {
        delete _data;
    }
}
