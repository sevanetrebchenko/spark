
#include <spark/utilitybox/logger/adapter/types/standard_output_adapter.h>

namespace Spark::UtilityBox::Logger {

    // Construct an adapter for a standard output stream.
    StandardOutputAdapter::StandardOutputAdapter(const char* name, std::ostream* stream) : IAdapter(name), _stream(stream) {
        GetConfiguration()->SetFormattingString("%x\n[%c] | %d | [ %s ]\n\tLogged through system: %n\n%x\n\t[ %t ] - %m\n\n");
        GetConfiguration()->SetTimestampFormattingString("%mm %ss %lms");
    }

    void StandardOutputAdapter::Initialize() {
        // Nothing to do here.
    }

    // Print all messages in the formatted messages buffer to the stream. Separates concurrent messages with a newline to aid in clarity.
    void StandardOutputAdapter::OutputMessage(std::vector<std::string> messages) {
        ++_logCount;

        for (auto& message : messages) {
            (*_stream) << message;
        }

        (*_stream).flush();
    }

    // Clear formatted messages and flush remaining messages into the stream.
    StandardOutputAdapter::~StandardOutputAdapter() {
        (*_stream).flush();
    }

}

