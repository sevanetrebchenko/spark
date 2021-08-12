
#include "spark/logger/adapter/adapter_configuration.h"

namespace Spark::Logger {

    AdapterConfiguration::AdapterConfiguration(std::string n) : globalFormattingString(R"("%x\n[%c] | [ %d ] | [ %s ]\n\t%f : \n%x\n\t[ %t ] - %m\n\n")"),
                                                                timestampFormattingString("%m %s %l"),
                                                                calendarFormatString("%A %d, %B %Y - %H:%M:%S"),
                                                                locationFormatString("in %f, %u:%l"),
                                                                name(std::move(n)),
                                                                severityCutoff(LogSeverity::DEBUG),
                                                                wrapMessage(true),
                                                                wrapLimit(100),
                                                                separatorLength(100)
                                                                {
    }

}
