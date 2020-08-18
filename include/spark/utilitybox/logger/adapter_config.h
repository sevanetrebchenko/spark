
#ifndef SPARK_ADAPTER_CONFIG_H
#define SPARK_ADAPTER_CONFIG_H

#include <spark_pch.h>                     // std::queue
#include <utilitybox/logger/log_message.h> // LogMessage

#define TAB_SPACE "    "

namespace Spark {
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
                TAB,
                DASH,
                SPACE,
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
                    * Globally remove header format. Ensures at least one format remains, which will be the default.
                    */
                    void PopHeaderFormat();

                    /**
                    * Globally remove message format. Ensures at least one format remains, which will be the default.
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
                    * OnUpdate the maximum number of characters a log line can have. Leaves old/already logged messages
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
                    * OnUpdate the current calendar format string to use a new format in formatting calendar information
                    * in log messages.
                    * @param newFormat - Updated format string.
                    */
                    void SetCalendarFormatString(const std::string& newFormat);

                private:
                    // Storage for AdapterConfiguration data, back-end functionality, and helper functions.
                    class AdapterConfigurationData;
                    AdapterConfigurationData* _data;
            };

        } // namespace Logger
    } // namespace UtilityBox
} // namespace Spark

#endif // SPARK_ADAPTER_CONFIG_H
