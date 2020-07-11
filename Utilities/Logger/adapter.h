
#ifndef DATASTRUCTURES_ADAPTER_H
#define DATASTRUCTURES_ADAPTER_H

#include "../Tools/global_defines.h" // _NODISCARD_
#include "adapter_config.h"    // AdapterConfiguration
#include "timestamp.h"         // TimeStamp
#include <sstream>             // std::stringstream
#include <queue>               // std::queue
#include <string>              // std::string

namespace UtilityBox::Logger {

    class Adapter {
        public:
            /**
             * Construct an adapter with default formatting with a given name.
             * @param name - Name for the adapter to have.
             */
            explicit Adapter(std::string&& name = "Not provided.");

            /**
             * Destructor.
             */
            virtual ~Adapter();

            /**
             * Process a message given a void pointer provided by the LoggingHub and output formatted strings into
             * the _formattedMessages queue to be outputted. Information about the message can be accessed through
             * LoggingHub functions.
             * @param messageAddress - Pointer to the message data stored in the LoggingHub.
             */
            virtual void ProcessMessage(void* messageAddress);

            /**
             * Output messages from the adapter into the desired location. Log counters should be updated during the
             * processing of this function to be reflected in log messages correctly.
             */
            virtual void OutputMessage() = 0;

            /**
             * Emplaces pre-processed error messages directly into the Adapter's store of formatted messages and
             * directly outputs them using the OutputMessage function.
             * @param processedErrorMessages - Structure of formatted messages to be printed.
             */
            void OutputErrorMessage(std::queue<std::string> processedErrorMessages);

            /**
             * Get the configuration for this adapter.
             * @return Adapter configuration instance.
             */
            _NODISCARD_ AdapterConfiguration& GetConfiguration();

            /**
             * Get the name for this adapter.
             * @return Adapter name.
             */
            _NODISCARD_ const std::string& GetName();

            /**
             * Get the current LogCount for this adapter.
             * @return Adapter log count.
             */
            _NODISCARD_ const int& GetLogCount() const;

        // Data and functions that are inherited - presents default behavior for processing messages.
        protected:
            /**
             * Given the desired header format provided through the AdapterConfiguration and data accessed through
             * the LoggingHub with the messageAddress, define what is added to the output message with each header
             * format element.
             * @param messageAddress - Pointer to the message data stored in the LoggingHub.
             */
            virtual void ConstructMessageHeader(void* messageAddress);

            /**
             * Given the desired message format provided through the AdapterConfiguration and data accessed through
             * the LoggingHub with the messageAddress, define what is added to the output message with each message
             * format element.
             * @param messageAddress - Pointer to the message data stored in the LoggingHub.
             */
            virtual void ConstructMessageBody(void* messageAddress);

            /**
             * Creates a block of text by formatting the given message. Wraps message contents to a hard character
             * limit. Should the message not fit on a single line, additional lines are aligned with the start of
             * the first line.
             * @param message         - Message to format.
             * @param characterStart  - Number of characters to offset the beginning of the message by.
             * @param characterEnd    - Maximum number of characters each line can be.
             * @return Vector of formatted messages.
             */
            _NODISCARD_ virtual std::vector<std::string> FormatMessage(const std::string& message, int characterStart, int characterEnd);

            /**
             * Formats a string to hold the calendar information based on the calendarFormat field in the adapter
             * configuration. Calendar information (by default) contains day of the week, date, month, year, and time
             * of the message.
             * @return String containing formatted calendar information.
             */
            _NODISCARD_ virtual std::string FormatCalendarInformation();

            /**
             * Format a counter representing the current message number through this adapter.
             * @return String containing formatted message number.
             */
            _NODISCARD_ virtual std::string FormatLogCounter();

            /**
             * Format the severity of this message.
             * @param messageSeverity - Severity to format
             * @return String containing formatted message severity.
             */
            _NODISCARD_ virtual std::string FormatSeverity(const LogMessageSeverity& messageSeverity);

            /**
             * Format the timestamp of this message.
             * @param timeStamp - TimeStamp object holding timestamp information for this log message.
             * @return String containing formatted timestamp.
             */
            _NODISCARD_ virtual std::string FormatTimestamp(const Timing::TimeStamp& timeStamp);

        #ifdef DEBUG_MESSAGES
            /**
             * Format the debug information (file, function, and line number where Supply provided the message).
             * @param debugLogRecord - Message's debug data store.
             * @return String containing formatted debug information.
             */
            _NODISCARD_ virtual std::string FormatDebugInformation(const LogMessage::DBG_LOG_RECORD& debugLogRecord);
        #endif

            /**
             * Format a line of dashes of a given length.
             * @param lineLength - Desired line length.
             * @return String containing formatted line.
             */
            _NODISCARD_ virtual std::string FormatLine(const int& lineLength);

            AdapterConfiguration _config;               // Adapter's configuration.
            std::string _adapterName;                   // Adapter's name.
            int _logCount;                              // Number of messages logged through this adapter.
            std::queue<std::string> _formattedMessages; // Intermediate storage for formatted messages to be stored for later printing.

        private:
            std::stringstream _format; // Stringstream for construction/formatting message headers/body.

            // Storage for Adapter data, back-end functionality, and helper functions.
            class AdapterData;
            std::unique_ptr<AdapterData> _data;
    };
}

#endif //DATASTRUCTURES_ADAPTER_H
