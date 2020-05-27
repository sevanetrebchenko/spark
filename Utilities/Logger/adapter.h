#ifndef DATASTRUCTURES_ADAPTER_H
#define DATASTRUCTURES_ADAPTER_H

#include <string>
#include "log_message.h"
#include <fstream>
#include <sstream>

namespace UtilityBox {
    namespace Logger {
        struct AdapterConfiguration {
            AdapterConfiguration() = default;
            ~AdapterConfiguration() = default;

            bool includeTimestamp = true;
            bool includeMessageSeverity = true;
            bool includeCalendarInfo = true;
            bool includeLogCount = true;

            LogMessageSeverity severityCutoff = LogMessageSeverity::WARNING;
            std::string calendarFormat = "[%A %d, %B %Y - %H:%M:%S]";
        };

        class Adapter {
            public:
                explicit Adapter(std::string&& name);
                virtual ~Adapter();
                virtual void ProcessMessage(void* messageData) = 0;
                virtual void OutputMessage() = 0;

                AdapterConfiguration& GetConfiguration();
                const std::string& GetName();

            protected:
                const char* FormatCalendarInformation();
                const char* FormatLogCounter();
                const char* FormatSeverity(LogMessageSeverity messageSeverity);
                const char* FormatTimestamp(const Timing::TimeStamp& timeStamp);

                AdapterConfiguration _config;
                std::string _adapterName;
                unsigned _logCount;

            private:
                class AdapterData;
                std::unique_ptr<AdapterData> _data;
        };

        class FileAdapter : public Adapter {
            public:
                explicit FileAdapter(std::string&& name, std::string&& filename = "");
                ~FileAdapter() override;

                void ProcessMessage(void* messageAddress) override;
                void OutputMessage() override;

            private:
                void LogInitializationMessage();

                std::vector<std::string> _formattedMessages;
                std::string _filename;
                std::stringstream _format;
                std::ofstream _logger;
        };
    }
}



#endif //DATASTRUCTURES_ADAPTER_H
