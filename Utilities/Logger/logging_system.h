#ifndef DATASTRUCTURES_LOGGING_SYSTEM_H
#define DATASTRUCTURES_LOGGING_SYSTEM_H

#include "log_message.h"

namespace UtilityBox {
    namespace Logger {
        class LoggingSystem {
            public:
                explicit LoggingSystem(std::string &&name = "");
                ~LoggingSystem();
                void Log(LogMessage *message);

            private:
                // Opaque pointer to abstract data
                class LoggingSystemData;
                std::unique_ptr <LoggingSystemData> _data;
        };
    }
}


#endif //DATASTRUCTURES_LOGGING_SYSTEM_H
