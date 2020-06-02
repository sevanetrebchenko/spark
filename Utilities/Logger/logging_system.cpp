//
// Created by seva on 5/25/20.
//

#include "logging_system.h"
#include "logger.h"

namespace UtilityBox {
    namespace Logger {
        class LoggingSystem::LoggingSystemData {
            public:
                explicit LoggingSystemData(std::string&& name);

                std::string _systemName;
        };

        LoggingSystem::LoggingSystemData::LoggingSystemData(std::string&& name) : _systemName(std::move(name)) {
        }

        LoggingSystem::LoggingSystem(std::string&& name) : _data(std::make_unique<LoggingSystemData>(std::move(name))) {
            // nothing to do here
        }

        LoggingSystem::~LoggingSystem() {
            _data.reset();
        }

        void LoggingSystem::Log(LogMessage* message) {
            LoggingHub::GetInstance().SendMessage(std::move(*message), _data->_systemName);
            delete message;
        }
    }
}
