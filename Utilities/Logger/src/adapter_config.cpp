
#include "../include/adapter_config.h" // AdapterConfiguration

namespace UtilityBox::Logger {
    // Constructor. Establishes default formats for headers and messages.
    AdapterConfiguration::AdapterConfiguration() {
        ConstructDefaultHeaderFormat();
        ConstructDefaultMessageFormat();
    }

    // Destructor. Cleans all header and message formats.
    AdapterConfiguration::~AdapterConfiguration() {
        while (!_headerFormats.empty()) {
            _headerFormats.pop();
        }

        while (!_messageFormats.empty()) {
            _messageFormats.pop();
        }
    }

    // Globally add a new header format.
    void AdapterConfiguration::PushHeaderFormat(const std::queue<HeaderFormatElement>& newFormat) {
        _headerFormats.push(newFormat);
    }

    // Globally add a new message format.
    void AdapterConfiguration::PushMessageFormat(const std::queue<MessageFormatElement>& newFormat) {
        _messageFormats.push(newFormat);
    }

    // Globally remove header format. Ensures at least one format remains (default).
    void AdapterConfiguration::PopHeaderFormat() {
        // Remove top-most format if it is not the only format.
        if (_headerFormats.size() > 1) {
            _headerFormats.pop();
        }
    }

    // Globally remove message format. Ensures at least one format remains (default).
    void AdapterConfiguration::PopMessageFormat() {
        // Remove top-most format if it is not the only format.
        if (_messageFormats.size() > 1) {
            _messageFormats.pop();
        }
    }

    // Gets a copy of the top-most header format - free to modify.
    std::queue<HeaderFormatElement> AdapterConfiguration::GetHeaderFormat() {
        return _headerFormats.top();
    }

    // Gets a copy of the top-most message format.
    std::queue<MessageFormatElement> AdapterConfiguration::GetMessageFormat() {
        return _messageFormats.top();
    }

    // Get the maximum number of characters a log line can have.
    const int &AdapterConfiguration::GetMessageWrapLimit() const {
        return _messageWrapLimit;
    }

    // Update the maximum number of characters a log line can have. Leaves old/already logged messages unchanged.
    void AdapterConfiguration::SetMessageWrapLimit(const int& newLimit) {
        _messageWrapLimit = newLimit;
    }

    // Get the minimum severity a log message must have to be logged through the adapter.
    const LogMessageSeverity &AdapterConfiguration::GetMessageSeverityCutoff() const {
        return _severityCutoff;
    }

    // Set the minimum severity a log message must have to be logged through the adapter.
    void AdapterConfiguration::SetMessageSeverityCutoff(const LogMessageSeverity& newCutoff) {
        _severityCutoff = newCutoff;
    }

    // Get the current calendar format string used in formatting calendar information for log messages.
    const std::string &AdapterConfiguration::GetCalendarFormatString() const {
        return _calendarFormat;
    }

    // Update the current calendar format string to use a new format in formatting calendar information in log messages.
    void AdapterConfiguration::SetCalendarFormatString(const std::string& newFormat) {
        if (_calendarFormat != newFormat) {
            _calendarFormat = newFormat;
        }
    }

    // Construct default global header format. If no custom formats are provided, this one is used.
    void AdapterConfiguration::ConstructDefaultHeaderFormat() {
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
    void AdapterConfiguration::ConstructDefaultMessageFormat() {
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
}
