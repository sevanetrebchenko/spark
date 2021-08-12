
#include "spark/logger/adapter/adapter_configuration.h"

namespace Spark::Logger {

    AdapterConfiguration::AdapterConfiguration(std::string n) : globalFormattingString("[ %t ] %s - %m [ %f ]\n"),
                                                                timestampFormattingString("%m %s %l"),
                                                                calendarFormatString("%A %d, %B %Y - %H:%M:%S"),
                                                                locationFormatString("in %f -> %u:%l"),
                                                                name(std::move(n)),
                                                                severityCutoff(LogSeverity::DEBUG),
                                                                wrapMessage(false),
                                                                wrapLimit(120),
                                                                separatorLength(120)
                                                                {
    }

}
