#ifndef DATASTRUCTURES_ADAPTER_CONFIG_H
#define DATASTRUCTURES_ADAPTER_CONFIG_H

#include <queue>
#include <stack>
#include "log_message.h"

#define TAB_SPACE "    "

namespace UtilityBox {
    namespace Logger {
        enum class HeaderFormatElement {
            NEWLINE,
            LOGCOUNT,
            DATE,
            SEVERITY,
            SYSTEM,
            SEPARATOR,
            TAB,
            BAR
        };

        enum class MessageFormatElement {
            NEWLINE,
            TIMESTAMP,
            MESSAGE,
            DEBUGINFO,
            TAB,
            ELLIPSIS,
            DASH,
            BAR
        };

        class AdapterConfiguration {
            public:
                AdapterConfiguration();
                ~AdapterConfiguration();

                // formats
                void AddCustomHeaderFormatStructure(const std::queue<HeaderFormatElement>& newFormat);
                void AddCustomMessageFormatStructure(const std::queue<MessageFormatElement>& newFormat);
                void RevertHeaderFormat();
                void RevertMessageFormat();

                std::queue<HeaderFormatElement> GetHeaderFormat();
                std::queue<HeaderFormatElement> GetDefaultHeaderFormat();
                std::queue<MessageFormatElement> GetMessageFormat();
                std::queue<MessageFormatElement> GetDefaultMessageFormat();

                // other data
                const int& GetMessageWrapLimit() const;
                void SetMessageWrapLimit(const int& newLimit);
                const LogMessageSeverity& GetMessageSeverityCutoff() const;
                void SetMessageSeverityCutoff(const LogMessageSeverity& newCutoff);
                const std::string& GetCalendarFormatString() const;
                void SetCalendarFormatString(const std::string& newFormat);

            private:
                class AdapterConfigurationData;
                std::unique_ptr<AdapterConfigurationData> _data;

                int _messageWrapLimit = 100;
                LogMessageSeverity _severityCutoff = LogMessageSeverity::DEBUG;
                std::string _calendarFormat = "[%A %d, %B %Y - %H:%M:%S]";

                // custom formats
                std::stack<std::queue<HeaderFormatElement>> _headerFormat;
                std::stack<std::queue<MessageFormatElement>> _messageFormat;
        };
    }
}


#endif //DATASTRUCTURES_ADAPTER_CONFIG_H
