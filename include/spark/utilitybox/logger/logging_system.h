
#ifndef SPARK_LOGGING_SYSTEM_H
#define SPARK_LOGGING_SYSTEM_H

#include "log_message.h" // LogMessage

namespace Spark {
    namespace UtilityBox {
        namespace Logger {

            class LoggingSystem {
                public:
                    /**
                    * Construct a LoggingSystem instance with a desired name.
                    * @param name - Name for this LoggingSystem.
                    */
                    explicit LoggingSystem(std::string name = "Not provided");

                    /**
                    * Destructor.
                    */
                    ~LoggingSystem() = default;

                    /**
                    * Log a message through this LoggingSystem. Note: Resources associated with the passed message are
                    * automatically cleaned up.
                    * @param message - Message to log.
                    */
                    void Log(LogMessage* message) const;

                    void Log(LogMessage& message) const;

                private:
                    std::string _name; // Logging system's name.
            };

        } // namespace Logger
    } // namespace UtilityBox
} // namespace Spark


#endif // SPARK_LOGGING_SYSTEM_H
