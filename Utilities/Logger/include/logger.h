
#ifndef DATASTRUCTURES_LOGGER_H
#define DATASTRUCTURES_LOGGER_H

#include "log_message.h"       // LogMessage
#include "logging_system.h"    // LoggingSystem
#include "adapter.h"           // Adapter
#include "../../Tools/global_defines.h" // _NODISCARD_
#include <vector>              // std::vector
#include <string>              // std::string
#include <chrono>              // std::chrono
#include <memory>              // std::unique_ptr

namespace UtilityBox::Logger {
    class LoggingHub {
        public:
            /**
             * Get a singleton LoggingHub instance.
             * @return Reference to the singleton LoggingHub instance.
             */
            static LoggingHub& GetInstance();

            /**
             * Direct call to initialize the necessary data for the LoggingHub to function properly. If not called,
             * will automatically be called before any messages are logged. Timestamps for log messages start at
             * LoggingHub initialization time.
             */
            static void Initialize();

            /**
             * Attach a custom adapter to process and receive messages through the LoggingHub.
             * @param adapter - Pointer to custom adapter.
             */
            void AttachCustomAdapter(Adapter* adapter);

            /**
             * Retrieve a custom adapter with a provided name.
             * @param name - Name of the desired adapter.
             * @return On success: Adapter is found, returns a pointer to it.
             *         On failure: Adapter is not found, returns a null pointer.
             */
            _NODISCARD_ Adapter* GetCustomAdapter(const std::string& name);

            /**
             * Used to verify the message pointer (provided to adapters' ProcessMessage function) before allowing access
             * to internal message data.
             * @param messageAddress - Pointer (provided to adapters' ProcessMessage function).
             * @return On success: true (valid pointer).
             *         On failure: false (invalid pointer).
             */
            _NODISCARD_ bool VerifyDataPointer(void* messageAddress);

            /**
             * Given a message pointer, safely get the log records of the underlying message.
             * @param messageAddress - Address of the message, provided to adapters' ProcessMessage function.
             * @return Read-only reference to the message store.
             */
            _NODISCARD_ const std::vector<LogMessage::LogRecord>& GetLogRecords(void* messageAddress);

            /**
             * Given a message pointer, safely get the message severity of the underlying message.
             * @param messageAddress - Address of the message, provided to adapters' ProcessMessage function.
             * @return Read-only reference to the message severity.
             */
            _NODISCARD_ const LogMessageSeverity& GetMessageSeverity(void* messageAddress);

            /**
             * Given a message pointer, safely get the name of the logging system that logged the message.
             * @param messageAddress - Address of the message, provided to adapters' ProcessMessage function.
             * @return Read-only reference to the logging system's name.
             */
            _NODISCARD_ const std::string& GetThroughLoggingSystem(void* messageAddress);

            /**
             * Get the initialization timestamp of the LoggingHub.
             * @return Timestamp of when the LoggingHub was initialized.
             */
            _NODISCARD_ const std::chrono::time_point<std::chrono::high_resolution_clock>& GetLoggingInitializationTime();

        private:
            // Allow logging systems to access SendMessage()
            friend class LoggingSystem;

            /**
             * Singleton constructor for an instance of the LoggingHub.
             */
            LoggingHub();

            /**
             * Destructor for the LoggingHub.
             */
            ~LoggingHub();

            /**
             * Direct call to shutdown the LoggingHub instance and flush all remaining messages.
             */
            static void Reset();

            /**
             * Send a message through to the LoggingHub. Function only available to logging systems.
             * @param message           - Message to pass through.
             * @param loggingSystemName - Name of the logging system being logged through.
             */
            void SendMessage(LogMessage* message, const std::string& loggingSystemName);

            // Storage for LoggingHub data, back-end functionality, and helper functions.
            class LoggingHubData;
            std::unique_ptr<LoggingHubData> _data;

            static LoggingHub* _loggingHub; // Pointer to the singleton instance of this class.
    };
}

#endif //DATASTRUCTURES_LOGGER_H
