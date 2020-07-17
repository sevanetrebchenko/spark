
#include "../../../include/spark/utilitybox/logger/adapter_config.h" // AdapterConfiguration

namespace Spark::UtilityBox::Logger {
    //------------------------------------------------------------------------------------------------------------------
    // ADAPTER CONFIGURATION DATA
    //------------------------------------------------------------------------------------------------------------------
    class AdapterConfiguration::AdapterConfigurationData {
        public:
            /**
             * Constructor. Establishes default formats for headers and messages.
             */
            AdapterConfigurationData();

            /**
             * Destructor. Cleans all header and message formats.
             */
            ~AdapterConfigurationData();

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

    // Constructor. Establishes default formats for headers and messages.
    AdapterConfiguration::AdapterConfigurationData::AdapterConfigurationData() {
        ConstructDefaultHeaderFormat();
        ConstructDefaultMessageFormat();
    }

    // Destructor. Cleans all header and message formats.
    AdapterConfiguration::AdapterConfigurationData::~AdapterConfigurationData() {
        while (!_headerFormats.empty()) {
            _headerFormats.pop();
        }

        while (!_messageFormats.empty()) {
            _messageFormats.pop();
        }
    }

    // Globally add a new header format.
    void AdapterConfiguration::AdapterConfigurationData::PushHeaderFormat(const std::queue<HeaderFormatElement>& newFormat) {
        _headerFormats.push(newFormat);
    }

    // Globally add a new message format.
    void AdapterConfiguration::AdapterConfigurationData::PushMessageFormat(const std::queue<MessageFormatElement>& newFormat) {
        _messageFormats.push(newFormat);
    }

    // Globally remove header format. Ensures at least one format remains (default).
    void AdapterConfiguration::AdapterConfigurationData::PopHeaderFormat() {
        // Remove top-most format if it is not the only format.
        if (_headerFormats.size() > 1) {
            _headerFormats.pop();
        }
    }

    // Globally remove message format. Ensures at least one format remains (default).
    void AdapterConfiguration::AdapterConfigurationData::PopMessageFormat() {
        // Remove top-most format if it is not the only format.
        if (_messageFormats.size() > 1) {
            _messageFormats.pop();
        }
    }

    // Gets a copy of the top-most header format - free to modify.
    std::queue<HeaderFormatElement> AdapterConfiguration::AdapterConfigurationData::GetHeaderFormat() {
        return _headerFormats.top();
    }

    // Gets a copy of the top-most message format.
    std::queue<MessageFormatElement> AdapterConfiguration::AdapterConfigurationData::GetMessageFormat() {
        return _messageFormats.top();
    }

    // Get the maximum number of characters a log line can have.
    const int &AdapterConfiguration::AdapterConfigurationData::GetMessageWrapLimit() const {
        return _messageWrapLimit;
    }

    // Update the maximum number of characters a log line can have. Leaves old/already logged messages unchanged.
    void AdapterConfiguration::AdapterConfigurationData::SetMessageWrapLimit(const int& newLimit) {
        _messageWrapLimit = newLimit;
    }

    // Get the minimum severity a log message must have to be logged through the adapter.
    const LogMessageSeverity &AdapterConfiguration::AdapterConfigurationData::GetMessageSeverityCutoff() const {
        return _severityCutoff;
    }

    // Set the minimum severity a log message must have to be logged through the adapter.
    void AdapterConfiguration::AdapterConfigurationData::SetMessageSeverityCutoff(const LogMessageSeverity& newCutoff) {
        _severityCutoff = newCutoff;
    }

    // Get the current calendar format string used in formatting calendar information for log messages.
    const std::string &AdapterConfiguration::AdapterConfigurationData::GetCalendarFormatString() const {
        return _calendarFormat;
    }

    // Update the current calendar format string to use a new format in formatting calendar information in log messages.
    void AdapterConfiguration::AdapterConfigurationData::SetCalendarFormatString(const std::string& newFormat) {
        if (_calendarFormat != newFormat) {
            _calendarFormat = newFormat;
        }
    }

    // Construct default global header format. If no custom formats are provided, this one is used.
    void AdapterConfiguration::AdapterConfigurationData::ConstructDefaultHeaderFormat() {
        std::queue<HeaderFormatElement> defaultHeaderFormat;

        // Formatting for section: ---------------------------------------------------------------------------------
        defaultHeaderFormat.push(HeaderFormatElement::SEPARATOR);
        defaultHeaderFormat.push(HeaderFormatElement::NEWLINE);

        // Formatting for section: [0000 0000] | [ DAY_OF_WEEK 00, MONTH 0000] | [ SEVERITY ].
        defaultHeaderFormat.push(HeaderFormatElement::LOGCOUNT);
        defaultHeaderFormat.push(HeaderFormatElement::BAR);
        defaultHeaderFormat.push(HeaderFormatElement::DATE);
        defaultHeaderFormat.push(HeaderFormatElement::BAR);
        defaultHeaderFormat.push(HeaderFormatElement::SEVERITY);
        defaultHeaderFormat.push(HeaderFormatElement::NEWLINE);

        // Formatting for section: Logged through system: SYSTEM_NAME.
        defaultHeaderFormat.push(HeaderFormatElement::TAB);
        defaultHeaderFormat.push(HeaderFormatElement::SYSTEM);
        defaultHeaderFormat.push(HeaderFormatElement::NEWLINE);

        // Formatting for section: ---------------------------------------------------------------------------------
        defaultHeaderFormat.push(HeaderFormatElement::SEPARATOR);
        defaultHeaderFormat.push(HeaderFormatElement::NEWLINE);

        _headerFormats.emplace(std::move(defaultHeaderFormat));
    }

    // Construct default global message format. If no custom formats are provided, this one is used.
    void AdapterConfiguration::AdapterConfigurationData::ConstructDefaultMessageFormat() {
        std::queue<MessageFormatElement> defaultMessageFormat;

        // Formatting for section: [000m 00s 0000ms] - Message goes here. If it spans multiple lines,
        //                                             additional lines are aligned.
        defaultMessageFormat.push(MessageFormatElement::TAB);
        defaultMessageFormat.push(MessageFormatElement::TIMESTAMP);
        defaultMessageFormat.push(MessageFormatElement::DASH);
        defaultMessageFormat.push(MessageFormatElement::MESSAGE);
        defaultMessageFormat.push(MessageFormatElement::NEWLINE);

        // Formatting for section:  : supplied from (FILE, FUNCTION:LINE_NUMBER)
    #ifdef DEBUG_MESSAGES
        defaultMessageFormat.push(MessageFormatElement::TAB);
        defaultMessageFormat.push(MessageFormatElement::TAB);
        defaultMessageFormat.push(MessageFormatElement::DEBUGINFO);
        defaultMessageFormat.push(MessageFormatElement::NEWLINE);
    #endif
        defaultMessageFormat.push(MessageFormatElement::NEWLINE);

        _messageFormats.emplace(std::move(defaultMessageFormat));
    }


    //------------------------------------------------------------------------------------------------------------------
    // ADAPTER CONFIGURATION
    //------------------------------------------------------------------------------------------------------------------
    // Establishes default formats for headers and messages.
    AdapterConfiguration::AdapterConfiguration() : _data(new AdapterConfigurationData()) {
        // Nothing to do here.
    }

    // Cleans all header and message formats.
    AdapterConfiguration::~AdapterConfiguration() {
        delete _data;
    }

    // Globally add a new header format.
    void AdapterConfiguration::PushHeaderFormat(const std::queue<HeaderFormatElement> &newFormat) {
        _data->PushHeaderFormat(newFormat);
    }

    // Globally add a new message format.
    void AdapterConfiguration::PushMessageFormat(const std::queue<MessageFormatElement> &newFormat) {
        _data->PushMessageFormat(newFormat);
    }

    // Globally remove header format. Ensures at least one format remains, which will be the default.
    void AdapterConfiguration::PopHeaderFormat() {
        _data->PopHeaderFormat();
    }

    // Globally remove message format. Ensures at least one format remains, which will be the default.
    void AdapterConfiguration::PopMessageFormat() {
        _data->PopMessageFormat();
    }

    // Gets a copy of the top-most header format.
    std::queue<HeaderFormatElement> AdapterConfiguration::GetHeaderFormat() {
        return _data->GetHeaderFormat();
    }

    // Gets a copy of the top-most message format.
    std::queue<MessageFormatElement> AdapterConfiguration::GetMessageFormat() {
        return _data->GetMessageFormat();
    }

    // Get the maximum number of characters a log line can have.
    const int &AdapterConfiguration::GetMessageWrapLimit() const {
        return _data->GetMessageWrapLimit();
    }

    // Update the maximum number of characters a log line can have. Leaves old/already logged messages unchanged.
    void AdapterConfiguration::SetMessageWrapLimit(const int &newLimit) {
        _data->SetMessageWrapLimit(newLimit);
    }

    // Get the minimum severity a log message must have to be logged through the adapter.
    const LogMessageSeverity &AdapterConfiguration::GetMessageSeverityCutoff() const {
        return _data->GetMessageSeverityCutoff();
    }

    // Set the minimum severity a log message must have to be logged through the adapter.
    void AdapterConfiguration::SetMessageSeverityCutoff(const LogMessageSeverity &newCutoff) {
        _data->SetMessageSeverityCutoff(newCutoff);
    }

    // Get the current calendar format string used in formatting calendar information for log messages.
    const std::string &AdapterConfiguration::GetCalendarFormatString() const {
        return _data->GetCalendarFormatString();
    }

    // Update the current calendar format string to use a new format in formatting calendar information in log messages.
    void AdapterConfiguration::SetCalendarFormatString(const std::string &newFormat) {
        _data->SetCalendarFormatString(newFormat);
    }
}
