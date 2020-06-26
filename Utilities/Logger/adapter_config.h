
#ifndef DATASTRUCTURES_ADAPTER_CONFIG_H
#define DATASTRUCTURES_ADAPTER_CONFIG_H

#include "log_message.h" // LogMessage
#include <queue>         // std::queue
#include <stack>         // std::stack

#define TAB_SPACE "    "

namespace UtilityBox {
    namespace Logger {

        // Element in formatting log message header.
        enum class HeaderFormatElement {
            NEWLINE,
            LOGCOUNT,
            DATE,
            SEVERITY,
            EXCEPTION,
            SYSTEM,
            SEPARATOR,
            TAB,
            DASH,
            BAR
        };

        // Element in formatting log message message body.
        enum class MessageFormatElement {
            NEWLINE,
            TIMESTAMP,
            MESSAGE,
            DEBUGINFO,
            TAB,
            DASH,
            BAR
        };

        class AdapterConfiguration {
            public:
                /**
                 * Constructor. Establishes default formats for headers and messages.
                 */
                AdapterConfiguration();

                /**
                 * Destructor. Cleans all header and message formats.
                 */
                ~AdapterConfiguration();

                /**
                 * Globally add a new header format.
                 * @param newFormat - New header format to use.
                 */
                void PushHeaderFormat(const std::queue<HeaderFormatElement>& newFormat);

                /**
                 * Globally add a new message format.
                 * @param newFormat - New message format to use.
                 */
                void PushMessageFormat(const std::queue<MessageFormatElement>& newFormat);

                /**
                 * Globally remove header format. Ensures at least one format remains (default).
                 */
                void PopHeaderFormat();

                /**
                 * Globally remove message format. Ensures at least one format remains (default).
                 */
                void PopMessageFormat();

                /**
                 * Gets a copy of the top-most header format.
                 * @return Copy of most recently added header format.
                 */
                std::queue<HeaderFormatElement> GetHeaderFormat();

                /**
                 * Gets a copy of the top-most message format.
                 * @return Copy of most recently added message format.
                 */
                std::queue<MessageFormatElement> GetMessageFormat();

                /**
                 * Get the maximum number of characters a log line can have.
                 * @return Maximum number of characters a log line can have.
                 */
                _NODISCARD_ const int& GetMessageWrapLimit() const;

                /**
                 * Update the maximum number of characters a log line can have. Leaves old/already logged messages
                 * unchanged.
                 * @param newLimit - New maximum number of characters a log line can have.
                 */
                void SetMessageWrapLimit(const int& newLimit);

                /**
                 * Get the minimum severity a log message must have to be logged through the adapter.
                 * @return Log message minimum severity.
                 */
                _NODISCARD_ const LogMessageSeverity& GetMessageSeverityCutoff() const;

                /**
                 * Set the minimum severity a log message must have to be logged through the adapter.
                 * @param newCutoff - New log message minimum severity.
                 */
                void SetMessageSeverityCutoff(const LogMessageSeverity& newCutoff);

                /**
                 * Get the current calendar format string used in formatting calendar information for log messages.
                 * @return Current calendar format string.
                 */
                _NODISCARD_ const std::string& GetCalendarFormatString() const;

                /**
                 * Update the current calendar format string to use a new format in formatting calendar information
                 * in log messages.
                 * @param newFormat - Updated format string.
                 */
                void SetCalendarFormatString(const std::string& newFormat);

            private:
                /**
                 * Construct default global header format. If no custom formats are provided, this one is used.
                 *   Default header format:
                 *   ---------------------------------------------------------------------------------
                 *    [0000 0000] | [DAY_OF_WEEK 00, MONTH 0000] | [ SEVERITY ]
                 *         Logged through system: SYSTEM_NAME.
                 *   ---------------------------------------------------------------------------------
                 */
                void ConstructDefaultHeaderFormat();

                /**
                 * Construct default global message format. If no custom formats are provided, this one is used.
                 *   Default message format:
                 *      [000m 00s 0000ms] - Message goes here. If it spans multiple lines,
                 *                          additional lines are aligned.
                 *          : supplied from (FILE, FUNCTION:LINE_NUMBER)
                 */
                void ConstructDefaultMessageFormat();

                int _messageWrapLimit = 100;                                    // Wrap lines to 100 character max.
                LogMessageSeverity _severityCutoff = LogMessageSeverity::DEBUG; // Allow DEBUG and higher to be logged.
                std::string _calendarFormat = "[%A %d, %B %Y - %H:%M:%S]";      // Default calendar format: [DAY_OF_WEEK, 00, MONTH 0000 - 00:00:00].
                std::stack<std::queue<HeaderFormatElement>> _headerFormats;     // Custom header formats.
                std::stack<std::queue<MessageFormatElement>> _messageFormats;   // Custom message formats.
        };
    }
}


#endif //DATASTRUCTURES_ADAPTER_CONFIG_H
