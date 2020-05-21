#pragma once
#ifndef DATASTRUCTURES_LOGGER_H
#define DATASTRUCTURES_LOGGER_H

#include "../assert_dev.h"
#include "timestamp.h"
#include <fstream>
#include <ostream>
#include <cstring>
#include <string>
#include <chrono>
#include <vector>
#include <iomanip>
#include "exception.h"

#define _NODISCARD_ [[nodiscard]]

namespace UtilityBox {
    namespace Logger {

        enum LogMessageSeverity {
            DEBUG,
            WARNING,
            ERROR,

            INVALID
        };


        class LogMessage {
            public:
                /**
                 * Constructor.
                 * @param messageSeverity - Severity of message.
                 */
                explicit LogMessage(LogMessageSeverity messageSeverity);

                /**
                 * Constructor.
                 * @param messageSeverity   - Severity of message.
                 * @param formatString      - Format string (printf-style).
                 * @param ...               - Additional parameters (printf-style).
                 */
                LogMessage(LogMessageSeverity messageSeverity, const char* formatString, ...);

                /**
                 * Destructor.
                 */
                ~LogMessage();

                /**
                 * Provide an additional message. Can be done multiple times for multi-line messages.
                 * @param formatString  - Format string (printf-style)
                 * @param ...           - Additional parameters (printf-style)
                 */
                void Supply(const char* formatString, ...);

                /**
                 * Enable printing messages to a given stream by using the stream insertion operator.
                 * @param stream    - Stream to print to
                 * @param message   - Message to print.
                 * @return
                 */
                friend std::ostream& operator<<(std::ostream& stream, const LogMessage& message);
            private:
                // Opaque pointer to abstract data - data should not be available
                class LogMessageData;
                std::unique_ptr<LogMessageData> _data;

                // Allow the logging system processing these messages to access the data
                friend class LoggingSystem;
        };

        class LoggingSystem {
            public:
                /**
                 * Constructor.
                 * @param name - Name for the new logging system. System name will be attached to log messages that are
                 *               sent through this instance.
                 */
                explicit LoggingSystem(std::string&& name = "");

                /**
                 * Destructor.
                 */
                ~LoggingSystem();

                /**
                 * Log an instance of the message class.
                 * @param message
                 */
                void Log(LogMessage* message);

                /**
                 * Log a message directly.
                 * @param messageSeverity
                 * @param formatString
                 * @param ...
                 */
                void Log(LogMessageSeverity messageSeverity, const char* formatString, ...);
            private:
                // Opaque pointer to abstract data
                struct LoggingSystemData;
                std::unique_ptr<LoggingSystemData> _data;
        };

        // Adapter begin
        class Adapter {
            public:
                Adapter(std::string name);
                virtual ~Adapter();
                virtual void ProcessMessage(void* messageData) = 0;
                virtual void OutputMessages() = 0;

            protected:

                /**
                 * Given the pointer provided by ProcessMessage, retrieves a pointer to the timestamp data of the messages.
                 * Message timestamps and logs are guaranteed to be at identical vector indices.
                 * @param messageAddress - Message data base pointer provided by ProcessMessage
                 * @return               - Upon success:
                 *                              Returns a pointer to the message log timestamps.
                 *                         Upon failure:
                 *                              Returns a nullptr.
                 */
                const std::vector<TimeStamp::TimeStamp>* GetMessageTimestamps(void* messageAddress);

                /**
                 * Given the pointer provided by ProcessMessage, retrieves a pointer to the logs data of the messages.
                 * Message timestamps and logs are guaranteed to be at identical vector indices.
                 * @param messageAddress - Message data base pointer provided by ProcessMessage
                 * @return               - Upon success:
                 *                              Returns a pointer to the message logs.
                 *                         Upon failure:
                 *                              Returns a nullptr.
                 */
                const std::vector<std::string>* GetMessageLogs(void* messageAddress);

                /**
                 * Given the pointer provided by ProcessMessage, retrieves the message severity.
                 * @param messageAddress - Message data base pointer provided by ProcessMessage
                 * @return               - Upon success:
                 *                              Returns the message severity.
                 *                         Upon failure:
                 *                              Returns 'INVALID'.
                 */
                LogMessageSeverity GetMessageSeverity(void* messageAddress);

                /**
                 * Given the pointer provided by ProcessMessage, retrieves the name of the system the message was
                 * processed through.
                 * @param messageAddress - Message data base pointer provided by ProcessMessage
                 * @return               - Upon success:
                 *                              Returns a pointer to the name of the system the message was processed
                 *                              through.
                 *                         Upon failure:
                 *                              Returns a nullptr.
                 */
                const std::string* GetMessageLoggingSystemName(void* messageAddress);

                /**
                 * Given the pointer provided by ProcessMessage, retrieves the count of this message through the
                 * system it was logged through.
                 * @param messageAddress - Message data base pointer provided by ProcessMessage
                 * @return               - Upon success:
                 *                              Returns the message count.
                 *                         Upon failure:
                 *                              Returns a nullptr.
                 */
                const unsigned* GetMessageLogCount(void* messageAddress);

                class AdapterData;
                std::unique_ptr<AdapterData> _data;
        };

//        class FileAdapter final : public Adapter {
//            public:
//                FileAdapter(std::string adapterName, std::string fileName);
//                virtual ~FileAdapter() override;
//                virtual void ProcessMessage(const void* const messageData) const override;
//
//            private:
//                class FileAdapterData;
//                std::unique_ptr<FileAdapterData> _data;
//        };

        void InitializeLoggingSystem();

        void AttachCustomAdapter(Adapter* adapter);
    }
}

#endif //DATASTRUCTURES_LOGGER_H
