
#include "../../../include/spark/utilitybox/logger/logging_system.h" // LoggingSystem functions
#include "../../../include/spark/utilitybox/logger/logger.h"         // SendMessage

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

    void LoggingSystem::Log(LogMessage& message) const {
        LoggingHub::GetInstance()->SendMessage(&message, _name);
    }
}
