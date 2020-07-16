
#include "../include/logging_system.h" // LoggingSystem functions
#include "../include/logger.h"         // SendMessage

namespace UtilityBox::Logger {
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
