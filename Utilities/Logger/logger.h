
#ifndef DATASTRUCTURES_LOGGER_H
#define DATASTRUCTURES_LOGGER_H

#include <fstream>

#include <cstring>
#include <string>
#include <chrono>
#include <vector>
#include <iomanip>
#include <queue>
#include <utility>
#include <thread>
#include <mutex>
#include <atomic>
#include <iostream>
#include "timestamp.h"
#include "log_message.h"
#include "logging_system.h"
#include "adapter.h"

#define _NODISCARD_ [[nodiscard]]

namespace UtilityBox {
    namespace Logger {
        class LoggingHub {
            public:
                static LoggingHub& GetInstance();
                static void Initialize();
                static void Reset();
                void AttachCustomAdapter(Adapter* adapter);

                const std::vector<LogMessage::LogRecord>& GetLogRecords(void* messageAddress);
                const LogMessageSeverity& GetMessageSeverity(void* messageAddress);
                const std::chrono::time_point<std::chrono::high_resolution_clock>& GetLoggingInitializationTime();

            private:
                class LoggingHubData;
                std::unique_ptr<LoggingHubData> _data;

                LoggingHub();
                ~LoggingHub();
                static LoggingHub* _loggingHub;
                std::chrono::time_point<std::chrono::high_resolution_clock> _initializationTime;

                // Allow logging systems to access SendMessage()
                friend class LoggingSystem;
                void SendMessage(LogMessage&& message);
        };
    }
}

#endif //DATASTRUCTURES_LOGGER_H
