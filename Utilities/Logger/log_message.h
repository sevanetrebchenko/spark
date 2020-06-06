
#ifndef DATASTRUCTURES_LOG_MESSAGE_H
#define DATASTRUCTURES_LOG_MESSAGE_H

#include "../global_defines.h" // _NODISCARD_
#include "timestamp.h"         // TimeStamp functions
#include <memory>              // std::unique_ptr
#include <vector>              // std::vector
#include <cstring>             // strrchr

#define DEBUG_MESSAGES

namespace UtilityBox {
    namespace Logger {

        enum class LogMessageSeverity {
            DEBUG,
            WARNING,
            SEVERE
        };

        class LogMessage {
            public:
                // Component of intermittent data storage
                struct LogRecord {
                    /**
                     * Construct a log record to store information about a logged message.
                     * @param message   - Logged message data.
                     * @param timestamp - Timestamp of the logged message.
                     */
                    LogRecord(std::string&& message, Timing::TimeStamp&& timestamp);

                    std::string _message;         // Logged message data.
                    Timing::TimeStamp _timestamp; // Timestamp of the logged message.
                };

                /**
                 * Construct an instance of a message to log.
                 * @param messageSeverity - Severity of the message (defaulted to DEBUG).
                 */
                explicit LogMessage(LogMessageSeverity messageSeverity = LogMessageSeverity::DEBUG);

                /**
                 * Log message destructor.
                 */
                ~LogMessage();

                /**
                 * Supply an additional message to be appended to this message instance. Allows for multi-line messages
                 * by supplying to the same message more than once.
                 * @param formatString - Printf-like formatting string for the message and additional parameters
                 * @param ...          - Printf-like list of variadic arguments.
                 */
                void Supply(const char* formatString, ...);

                /**
                 * Get a reference to the store of log record information this LogMessage instance holds.
                 * @return Reference to LogMessage data.
                 */
                _NODISCARD_ std::vector<LogRecord>& GetLogRecords();

                /**
                 * Get the severity of this message.
                 * @return Severity of this message.
                 */
                _NODISCARD_ LogMessageSeverity& GetMessageSeverity();

            private:
                // Storage for LogMessage data, back-end functionality, and helper functions.
                class LogMessageData;
                std::unique_ptr<LogMessageData> _data;

                std::vector<LogRecord> _logMessages; // Store of log record information
                LogMessageSeverity _messageSeverity; // Message severity
        };

    #ifdef DEBUG_MESSAGES
        // Class captures the caller by overwriting both LogMessage and Supply functions above and records debug information
        class DBG_LOG_MESSAGE {
            public:
                // Component of intermittent debug data storage
                struct DBG_LOG_RECORD {
                    /**
                     * Construct a debug log record to store callee information about a logged message.
                     * @param filename     - File name where Supply was called.
                     * @param functionName - Function from which Supply was called.
                     * @param lineNumber   - Line number where Supply was called.
                     */
                    DBG_LOG_RECORD(std::string&& filename, std::string&& functionName, int lineNumber);

                    std::string _fileName;     // Filename
                    std::string _functionName; // Function name
                    int _lineNumber;           // Line number
                };

                // Overwritten component of intermittent data storage
                struct LogRecord {
                    /**
                     * Construct a log record to store information about a logged message.
                     * @param message           - Logged message data.
                     * @param timestamp         - Timestamp of the logged message.
                     * @param calleeInformation - Debug information (filename, file, line number)
                     */
                    LogRecord(std::string&& message, Timing::TimeStamp&& timestamp, DBG_LOG_RECORD&& calleeInformation);

                    std::string _message;              // Logged message data.
                    Timing::TimeStamp _timestamp;      // Timestamp of the logged message.
                    DBG_LOG_RECORD _calleeInformation; // Debug log record.
                };

                /**
                 * Construct an instance of a message to log.
                 * @param messageSeverity - Severity of the message (defaulted to DEBUG).
                 */
                explicit DBG_LOG_MESSAGE(LogMessageSeverity messageSeverity = LogMessageSeverity::DEBUG);

                /**
                 * Log message destructor.
                 */
                ~DBG_LOG_MESSAGE();

                /**
                 * Supply an additional message to be appended to this message instance. Allows for multi-line messages
                 * by supplying to the same message more than once.
                 * @param callingFunction - Name of the function that called Supply (provided automatically by the macro).
                 * @param fileName        - Name of the file in which Supply was called (provided automatically by the macro).
                 * @param lineNumber      - Line number at which Supply was called (provided automatically by the macro).
                 * @param formatString    - Printf-like formatting string for the message and additional parameters
                 * @param ...             - Printf-like list of variadic arguments.
                 */
                void SUPPLY_DBG(std::string&& callingFunction, std::string&& fileName, int lineNumber, const char* formatString, ...);

                /**
                 * Get a reference to the store of log record information this LogMessage instance holds.
                 * @return Reference to LogMessage data.
                 */
                _NODISCARD_ std::vector<LogRecord>& GetLogRecords();

                /**
                 * Get the severity of this message.
                 * @return Severity of this message.
                 */
                _NODISCARD_ LogMessageSeverity& GetMessageSeverity();

            private:
                // Storage for LogMessage data, back-end functionality, and helper functions.
                class LogMessageData;
                std::unique_ptr<LogMessageData> _data;

                std::vector<LogRecord> _logMessages; // Store of log record information
                LogMessageSeverity _messageSeverity; // Message severity
        };

        // Shorten file name - get the last occurrence of the '/' character to only get the file name.
        #define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

        // Un-define the LogMessage class and Supply function to redefine the class and function to take more parameters.
        // As a result of this, if DEBUG_MESSAGES is defined, the supply function automatically takes in information
        // about the function that called Supply, in which file, and on which line number of that file (shown below).
        #undef LogMessage
        #define LogMessage DBG_LOG_MESSAGE
        #undef Supply
        #define Supply(formatString, ...) SUPPLY_DBG(__FUNCTION__, __FILENAME__, __LINE__, formatString, ##__VA_ARGS__)
    #endif
    }
}

#endif //DATASTRUCTURES_LOG_MESSAGE_H
