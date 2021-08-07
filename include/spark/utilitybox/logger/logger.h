
#ifndef SPARK_LOGGER_H
#define SPARK_LOGGER_H

#include "spark/core/utility.h"
#include "spark/utilitybox/tools/singleton.h"
#include "spark/utilitybox/logger/timestamp.h"
#include "spark/utilitybox/logger/log_record.h"
#include "spark/utilitybox/utility.h"
#include "spark/utilitybox/logger/adapter/adapter.h"

namespace Spark {
    namespace UtilityBox {

        class Logger : Singleton<Logger> {
            public:
                REGISTER_SINGLETON(Logger);

                void LogMessage(const std::string& file, const std::string& function, int lineNumber, LogSeverity messageSeverity, TimeStamp timeStamp, const char* formatString, ...);

                void AddAdapter(IAdapter* adapter);
                void RemoveAdapter(IAdapter* adapter);

            private:
                typedef std::queue<LogRecord> PrintingBuffer;

                Logger();
                ~Logger() override;

                PrintingBuffer& SwitchBuffers();

                // Processes and distributes messages.
                void DistributeMessages();

                NODISCARD std::string ProcessFormatString(const char* formatString, std::va_list argsList);
                NODISCARD std::vector<std::string> ProcessMessage(const AdapterConfiguration& adapterConfiguration, const LogRecord& logRecord);
                NODISCARD std::vector<std::string> FormatMessage(const std::string &message, int characterStart, bool wrap, int wrapLimit);
                NODISCARD std::string FormatSeverity(const LogSeverity &messageSeverity);
                NODISCARD std::string FormatTimestamp(const TimeStamp &timeStamp, const std::string& formatString);
                NODISCARD std::string FormatLocation(const std::string& file, const std::string& function, int lineNumber, const std::string& formatString);
                NODISCARD std::string FormatCalendarInformation(const std::string& formatString);
                NODISCARD std::string FormatLogCount();
                NODISCARD std::string FormatSeparator(unsigned separatorLength);

                unsigned processingBufferSize_;
                char* processingBuffer_;

                // Formatting.
                unsigned logCounter_;
                std::vector<IAdapter*> adapters_;

                // While one buffer is being emptied, it should NOT be printed to.
                PrintingBuffer buffer1_;
                PrintingBuffer buffer2_;

                std::mutex bufferMutex_;
                std::chrono::milliseconds asynchronousInterval_; // Interval at which thread switches buffers and processes messages.
                std::atomic<bool> distributeMessages_;
                std::atomic<std::queue<LogRecord>*> printingBufferLocation_;
                std::thread distributingThread_;
        };

    }
}

// Format: filename, function, line number, severity, timestamp, message.
#define LogDebug(formatString, ...) Spark::Singleton<Spark::UtilityBox::Logger>::GetInstance()->LogMessage( __FILENAME__, __func__, __LINE__, Spark::UtilityBox::LogSeverity::DEBUG, std::chrono::high_resolution_clock::now(), formatString, ##__VA_ARGS__ )
#define LogWarning(formatString, ...) Spark::Singleton<Spark::UtilityBox::Logger>::GetInstance()->LogMessage( __FILENAME__, __func__, __LINE__, Spark::UtilityBox::LogSeverity::WARNING, std::chrono::high_resolution_clock::now(), formatString, ##__VA_ARGS__ )
#define LogError(formatString, ...) Spark::Singleton<Spark::UtilityBox::Logger>::GetInstance()->LogMessage( __FILENAME__, __func__, __LINE__, Spark::UtilityBox::LogSeverity::ERROR, std::chrono::high_resolution_clock::now(), formatString, ##__VA_ARGS__ )

#endif // SPARK_LOGGER_H
