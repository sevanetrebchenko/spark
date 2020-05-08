
#ifndef DATASTRUCTURES_LOGGER_H
#define DATASTRUCTURES_LOGGER_H

#include <string>

namespace UtilityBox {
    namespace Logger {
        enum MessageSeverity {
            DEBUG,
            WARNING,
            ERROR
        };

        void logMessage(MessageSeverity, const char* formatString, ...);
    }
}


#endif //DATASTRUCTURES_LOGGER_H
