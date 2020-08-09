
#include "utilitybox/logger/logging_system.h" // LoggingSystem functions
#include "utilitybox/logger/logger.h"         // SendMessage
#include <spark_pch.h>

namespace Spark::UtilityBox::Logger {
    // Construct a LoggingSystem instance with a desired name.
    LoggingSystem::LoggingSystem(std::string name) : _name(std::move(name)) {
        // Nothing to do here.
    }

    // Log a message through this LoggingSystem.
    void LoggingSystem::Log(LogMessage* message) const {
        LoggingHub::GetInstance()->SendMessage(message, _name);
        delete message;
    }

    // Log a message through this LoggingSystem.
    void LoggingSystem::Log(LogMessage& message) const {
        LoggingHub::GetInstance()->SendMessage(&message, _name);
    }

    // Log a message directly through this LoggingSystem.
    void LoggingSystem::Log(LogMessageSeverity messageSeverity, const char *formatString, ...) const {
        LogMessage message { messageSeverity };

        std::va_list args;
        va_start(args, formatString);
        message.Supply(formatString, args);
        va_end(args);

        Log(message);
    }
}
