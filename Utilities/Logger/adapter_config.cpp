#include "adapter_config.h"

namespace UtilityBox {
    namespace Logger {

        class AdapterConfiguration::AdapterConfigurationData {
            public:
                AdapterConfigurationData();
                ~AdapterConfigurationData();

                std::queue<HeaderFormatElement> GetDefaultHeaderFormat();
                std::queue<MessageFormatElement> GetDefaultMessageFormat();

            private:
                void ConstructDefaultHeaderFormat();
                void ConstructDefaultMessageFormat();

                std::queue<HeaderFormatElement> _defaultHeaderFormat;
                std::queue<MessageFormatElement> _defaultMessageFormat;
        };

        AdapterConfiguration::AdapterConfigurationData::AdapterConfigurationData() {
            ConstructDefaultHeaderFormat();
            ConstructDefaultMessageFormat();
        }

        AdapterConfiguration::AdapterConfigurationData::~AdapterConfigurationData() {
            // clear normal format
            while (!_defaultHeaderFormat.empty()) {
                _defaultHeaderFormat.pop();
            }

            // clear error format
            while (!_defaultMessageFormat.empty()) {
                _defaultMessageFormat.pop();
            }
        }

        std::queue<HeaderFormatElement> AdapterConfiguration::AdapterConfigurationData::GetDefaultHeaderFormat() {
            return _defaultHeaderFormat;
        }

        std::queue<MessageFormatElement> AdapterConfiguration::AdapterConfigurationData::GetDefaultMessageFormat() {
            return _defaultMessageFormat;
        }

        void AdapterConfiguration::AdapterConfigurationData::ConstructDefaultHeaderFormat() {
            // default header format:
            //---------------------------------------------------------------------------------
            // [0000 0000] | [ DAY_OF_WEEK 00, MONTH 0000] | [ SEVERITY ]
            //      Logged through system: SYSTEM_NAME.
            //---------------------------------------------------------------------------------
            _defaultHeaderFormat.push(HeaderFormatElement::SEPARATOR);
            _defaultHeaderFormat.push(HeaderFormatElement::NEWLINE);

            _defaultHeaderFormat.push(HeaderFormatElement::LOGCOUNT);
            _defaultHeaderFormat.push(HeaderFormatElement::BAR);
            _defaultHeaderFormat.push(HeaderFormatElement::DATE);
            _defaultHeaderFormat.push(HeaderFormatElement::BAR);
            _defaultHeaderFormat.push(HeaderFormatElement::SEVERITY);
            _defaultHeaderFormat.push(HeaderFormatElement::NEWLINE);

            _defaultHeaderFormat.push(HeaderFormatElement::TAB);
            _defaultHeaderFormat.push(HeaderFormatElement::SYSTEM);
            _defaultHeaderFormat.push(HeaderFormatElement::NEWLINE);

            _defaultHeaderFormat.push(HeaderFormatElement::SEPARATOR);
            _defaultHeaderFormat.push(HeaderFormatElement::NEWLINE);
        }

        void AdapterConfiguration::AdapterConfigurationData::ConstructDefaultMessageFormat() {
            // default message format:
            //      [000m 00s 0000ms] - Message goes here. If it spans multiple lines,
            //                          additional lines are aligned.
            //          : supplied from (FILE, FUNCTION:LINE_NUMBER)
            _defaultMessageFormat.push(MessageFormatElement::TAB);
            _defaultMessageFormat.push(MessageFormatElement::TIMESTAMP);
            _defaultMessageFormat.push(MessageFormatElement::DASH);
            _defaultMessageFormat.push(MessageFormatElement::MESSAGE);
            _defaultMessageFormat.push(MessageFormatElement::NEWLINE);

#ifdef DEBUG_MESSAGES
            _defaultMessageFormat.push(MessageFormatElement::TAB);
            _defaultMessageFormat.push(MessageFormatElement::TAB);
            _defaultMessageFormat.push(MessageFormatElement::DEBUGINFO);
            _defaultMessageFormat.push(MessageFormatElement::NEWLINE);
#endif
            _defaultMessageFormat.push(MessageFormatElement::NEWLINE);
        }



        // Adapter configuration functions
        AdapterConfiguration::AdapterConfiguration() : _data(std::make_unique<AdapterConfigurationData>()) {
            _headerFormat.push(std::move(_data->GetDefaultHeaderFormat()));
            _messageFormat.push(std::move(_data->GetDefaultMessageFormat()));
        }

        AdapterConfiguration::~AdapterConfiguration() {
            _data.reset();
        }

        void AdapterConfiguration::AddCustomHeaderFormatStructure(const std::queue<HeaderFormatElement> &newFormat) {
            _headerFormat.push(newFormat);
        }

        void AdapterConfiguration::AddCustomMessageFormatStructure(const std::queue<MessageFormatElement> &newFormat) {
            _messageFormat.push(newFormat);
        }

        void AdapterConfiguration::ResetHeaderFormatToDefault() {
            while (_headerFormat.size() != 1) {
                _headerFormat.pop();
            }
        }

        void AdapterConfiguration::ResetMessageFormatToDefault() {
            while (_messageFormat.size() != 1) {
                _messageFormat.pop();
            }
        }

        void AdapterConfiguration::RevertHeaderFormat() {
            if (_headerFormat.size() != 1) {
                _headerFormat.pop();
            }
        }

        void AdapterConfiguration::RevertMessageFormat() {
            if (_messageFormat.size() != 1) {
                _messageFormat.pop();
            }
        }

        std::queue<HeaderFormatElement> AdapterConfiguration::GetHeaderFormat() {
            return _headerFormat.top();
        }

        std::queue<MessageFormatElement> AdapterConfiguration::GetMessageFormat() {
            return _messageFormat.top();
        }

        const int &AdapterConfiguration::GetMessageWrapLimit() const {
            return _messageWrapLimit;
        }

        void AdapterConfiguration::SetMessageWrapLimit(const int &newLimit) {
            _messageWrapLimit = newLimit;
        }

        const LogMessageSeverity &AdapterConfiguration::GetMessageSeverityCutoff() const {
            return _severityCutoff;
        }

        void AdapterConfiguration::SetMessageSeverityCutoff(const LogMessageSeverity &newCutoff) {
            _severityCutoff = newCutoff;
        }

        const std::string &AdapterConfiguration::GetCalendarFormatString() const {
            return _calendarFormat;
        }

        void AdapterConfiguration::SetCalendarFormatString(const std::string &newFormat) {
            if (_calendarFormat != newFormat) {
                _calendarFormat = newFormat;
            }
        }
    }
}
