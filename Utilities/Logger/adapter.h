#ifndef DATASTRUCTURES_ADAPTER_H
#define DATASTRUCTURES_ADAPTER_H

#include <string>
#include "log_message.h"
#include "adapter_config.h"
#include <fstream>
#include <sstream>
#include <queue>

namespace UtilityBox {
    namespace Logger {
        class Adapter {
            public:
                explicit Adapter(std::string&& name);
                virtual ~Adapter();

                virtual void ProcessMessage(void* messageAddress);
                virtual void OutputMessage() = 0;
                void ClearMessages();
                void OutputErrorMessage(std::queue<std::string>&& processedErrorMessages);

                AdapterConfiguration& GetConfiguration();
                const std::string& GetName();

            protected:
                virtual void FormatHeader(void* messageAddress);
                virtual void FormatMessages(void* messageAddress);

                virtual std::vector<std::string> FormatMessage(const std::string& message, int timestampLength, unsigned lineLength);
                virtual std::string FormatCalendarInformation();
                virtual std::string FormatLogCounter();
                virtual std::string FormatSeverity(LogMessageSeverity messageSeverity);
                virtual std::string FormatTimestamp(const Timing::TimeStamp& timeStamp);
#ifdef DEBUG_MESSAGES
                virtual std::string FormatDebugInformation(const LogMessage::DBG_LOG_RECORD& debugLogRecord);
#endif
                virtual std::string FormatLine(const int& lineLength);

                AdapterConfiguration _config;
                std::string _adapterName;
                unsigned _logCount;
                std::queue<std::string> _formattedMessages;

            private:
                std::stringstream _format;
                class AdapterData;
                std::unique_ptr<AdapterData> _data;
        };
    }
}



#endif //DATASTRUCTURES_ADAPTER_H
