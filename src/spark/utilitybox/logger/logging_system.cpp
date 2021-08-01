
#include <spark/core/service_locator.h>       // ServiceLocator
#include <spark/utilitybox/logger/logging_system.h> // LoggingSystem

namespace Spark::UtilityBox::Logger {
    class LoggingSystem::LoggingSystemData {
        public:
            explicit LoggingSystemData(const char* name);

            NODISCARD const char* ProcessMessage(const char* formatString, std::va_list argsList);
            NODISCARD const char* GetSystemName() const;

        private:
            const char* _systemName;

            unsigned _processingBufferSize; // Size of the processing buffer.
            char* _processingBuffer;        // Pointer to the processing buffer.
    };

    LoggingSystem::LoggingSystemData::LoggingSystemData(const char* name) : _systemName(name), _processingBufferSize(64u) {
        _processingBuffer = new char[_processingBufferSize];
    }

    const char* LoggingSystem::LoggingSystemData::ProcessMessage(const char *formatString, std::va_list argsList) {
        unsigned currentBufferSize = _processingBufferSize;

        // Copy args list to not modify passed parameters (yet).
        std::va_list argsCopy;
        va_copy(argsCopy, argsList);

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
            _processingBuffer = new char[_processingBufferSize];
        }

        vsnprintf(_processingBuffer, _processingBufferSize, formatString, argsList);

        return _processingBuffer;
    }

    const char *LoggingSystem::LoggingSystemData::GetSystemName() const {
        return _systemName;
    }


    //------------------------------------------------------------------------------------------------------------------
    // LOGGING SYSTEM
    //------------------------------------------------------------------------------------------------------------------
    // Construct a LoggingSystem instance with a desired name.
    LoggingSystem::LoggingSystem(const char* name) : _data(new LoggingSystemData(name)) {
        // Nothing to do here.
    }

    // Log a message directly through this LoggingSystem.
    void LoggingSystem::Log(LogMessageSeverity messageSeverity, const char *formatString, std::va_list argsList) const {
        LogRecord logRecord;
        logRecord.message = _data->ProcessMessage(formatString, argsList);
        logRecord.messageSeverity = messageSeverity;
        logRecord.loggingSystemName = _data->GetSystemName();

        Spark::ServiceLocator::GetLoggingHub()->SendMessage(std::move(logRecord));
    }

    // Log a message indirectly through ILoggable interface.
    void LoggingSystem::Log(LogMessageSeverity messageSeverity, const char *formatString, ...) const {
        std::va_list argsList;
        va_start(argsList, formatString);
        Log(messageSeverity, formatString, argsList);
        va_end(argsList);
    }
}
