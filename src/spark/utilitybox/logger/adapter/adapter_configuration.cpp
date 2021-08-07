
#include "spark/utilitybox/logger/adapter/adapter_configuration.h"

namespace Spark::UtilityBox {

    AdapterConfiguration::AdapterConfiguration(const std::string& n) : globalFormattingString("\"%x\\n[%c] | [ %d ] | [ %s ]\\n\\t%f : \\n%x\\n\\t[ %t ] - %m\\n\\n\""),
                                                                       timestampFormattingString("%m %s %l"),
                                                                       calendarFormatString("%A %d, %B %Y - %H:%M:%S"),
                                                                       locationFormatString("in %f, %u:%l"),
                                                                       name(n),
                                                                       severityCutoff(LogSeverity::DEBUG),
                                                                       wrapMessage(true),
                                                                       wrapLimit(100),
                                                                       separatorLength(100)
                                                                       {
    }

}
